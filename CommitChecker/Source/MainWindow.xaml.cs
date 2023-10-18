using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using static System.Net.Mime.MediaTypeNames;

namespace CommitChecker
{
    public class TargetPlatformData
    {
        public string platformName = null;
        public string configurationName = null;

        public Process process = null;
        public string compilingScriptPath = null;
        public string compileProcessFullOutput = null;
        public string compileProcessErrorOutput = null;

        public TargetPlatformData() 
        {}

        public void StartCompilingProcess()
        {
            FindCompilingScriptPathForPlatform();
            Debug.Assert((compilingScriptPath != null && compilingScriptPath.Count() > 0), "Compiling script for platform " + platformName + " & configuration " + configurationName + " doesn't exist?");

            process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo.CreateNoWindow = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.UseShellExecute = false;

            // Windows - can just use the regulard start command to execute the build script.
            if (platformName == PlatformData.Windows)
            {
                process.StartInfo.FileName = "cmd.exe";
                process.StartInfo.Arguments = "/c " + compilingScriptPath;
            }
            // Linux - use the WSL instead to run the build script.
            else if (platformName == PlatformData.Linux)
            {
                process.StartInfo.FileName = "powershell.exe";
                process.StartInfo.Arguments = @"-executionpolicy unrestricted wsl . " + compilingScriptPath;
            }

            bool processStartedSuccessfully = process.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to start " + platformName + " compile process. \nPlease check the arguments!");

            // Reset any output/error strings
            compileProcessFullOutput = "";
            compileProcessErrorOutput = "";

            process.OutputDataReceived += (object sender, DataReceivedEventArgs e) =>
            {
                if (e.Data != null && e.Data.Count() > 0)
                {
                    Console.WriteLine(e.Data);
                    compileProcessFullOutput += e.Data;
                    compileProcessFullOutput += "\n";
                }
            };
            process.BeginOutputReadLine();

            process.ErrorDataReceived += (object sender, DataReceivedEventArgs e) =>
            {
                if (e.Data != null && e.Data.Count() > 0)
                {
                    Console.WriteLine(e.Data);
                    compileProcessErrorOutput += e.Data;
                    compileProcessErrorOutput += "\n";
                }
            };
            process.BeginErrorReadLine();

            process.Exited += Process_Exited;
        }

        private void Process_Exited(object sender, EventArgs e)
        {
            Console.WriteLine(platformName + " " + configurationName + " ExitCode: {0}", process.ExitCode);
            process.Close();
            process = null;
        }

        public bool IsProcessRunning()
        {
            return process != null;
        }

        public bool GeneratedAnyWarningsOrErrors()
        {
            return (compileProcessErrorOutput != null && compileProcessErrorOutput.Count() > 0);
        }

        private void GivePermissionForCompileScripts()
        {
            Debug.Assert(platformName != PlatformData.Linux, "Trying to give file permission on a Windows platform?");

            process = new Process();
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.FileName = "powershell.exe";
            process.StartInfo.Arguments = @"-executionpolicy unrestricted wsl chmod +x " + compilingScriptPath;
            process.StartInfo.WorkingDirectory = System.AppDomain.CurrentDomain.BaseDirectory.ToString();

            bool processStartedSuccessfully = process.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to start " + platformName + " compile process. \nPlease check the arguments!");
        }

        private string FindCompilingScriptPathForPlatform()
        {
            if (compilingScriptPath == null)
            {
                string currentPath = "";
                var filePaths = Directory.GetFiles(System.AppDomain.CurrentDomain.BaseDirectory);
                foreach (var item in filePaths)
                {
                    if (item.Contains(platformName) && item.Contains(configurationName))
                    {
                        currentPath = System.IO.Path.GetFileName(item);
                        break;
                    }
                }

                Debug.Assert(currentPath != null, "No compiling script found for platform " + platformName + " & configuration " + configurationName);

                if (platformName == PlatformData.Windows)
                {
                    compilingScriptPath = currentPath;
                }
                // Need to replace any backslashes with forward slashes
                else if (platformName == PlatformData.Linux)
                {
                    string updatedScriptPath = currentPath.Replace("\\", "/");
                    compilingScriptPath = updatedScriptPath;

                    // For Linux, need to give the right file permissions.
                    GivePermissionForCompileScripts();
                }
            }

            return compilingScriptPath;
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public WarningOrErrorWindow warningOrErrorWindow = new WarningOrErrorWindow();
        private TargetsWindow targetsWindow = new TargetsWindow();
        private List<TargetPlatformData> targetsToCompile = new List<TargetPlatformData>();
        private DispatcherTimer compileTimer = new DispatcherTimer();
        private BitmapImage greenTickImage = new BitmapImage(new Uri(@"Assets/Images/GreenTick.png", UriKind.Relative));
        private BitmapImage redCrossImage = new BitmapImage(new Uri(@"Assets/Images/RedCross.png", UriKind.Relative));
        private bool compileSucceeded = false;

        public MainWindow()
        {
            InitializeComponent();

            // Want to enable this platform/configuration by default
            AddTargetToCompile(PlatformData.Linux, PlatformData.Debug);

            // Setup the timer here, but not starting it straight away, will be started once the user clicks on the "Compile" button.
            compileTimer.Tick += new EventHandler(OnCompileTimerTick);
            compileTimer.Interval = TimeSpan.FromSeconds(1);
        }

        public void ClearTargetsToCompile()
        {
            targetsToCompile.Clear();
        }

        public void AddTargetToCompile(string platformName, string configuration)
        {
            TargetPlatformData platformData = new TargetPlatformData();
            platformData.platformName = platformName;
            platformData.configurationName = configuration; 
            targetsToCompile.Add(platformData);
        }

        public void AddTargetToCompile(TargetPlatformData targetData)
        {
            targetsToCompile.Add(targetData);
        }

        public bool HasTargetToCompile(string platformName, string configurationName)
        {
            bool hasTarget = targetsToCompile.Any(target => (target.platformName == platformName && target.configurationName == configurationName));
            return hasTarget;
        }

        private void MainWindow_TitleBar_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void MainWindow_Button_Targets_OnClick(object sender, RoutedEventArgs e)
        {
            if (targetsWindow != null && targetsWindow.Visibility != Visibility.Visible)
            {
                targetsWindow.Show();
            }
        }

        private void StartCompiling()
        {
            if (targetsToCompile.Count > 0)
            {
                Image_CompileResults.Visibility = Visibility.Hidden;
                Button_Compile.Visibility = Visibility.Collapsed;
                Button_Compiling.Visibility = Visibility.Visible;

                if (warningOrErrorWindow != null)
                {
                    warningOrErrorWindow.TextBlock_WarningOrErrorsBox.Text = "";
                }

                targetsToCompile.ForEach(target =>
                {
                    target.StartCompilingProcess();
                });

                compileTimer.Start();
            }
            else
            {
                MessageBox.Show("Please select at least 1 platform/configuration from the Targets window.");
            }
        }

        private void OnCompilingFinished()
        {
            Console.WriteLine("All compiler processes have finished now!");

            Button_Compile.Visibility = Visibility.Visible;
            Button_Compiling.Visibility = Visibility.Collapsed;

            if (warningOrErrorWindow != null)
            {
                string allWarningOrErrorText = "";
                targetsToCompile.ForEach(target =>
                {
                    if (target.GeneratedAnyWarningsOrErrors())
                    {
                        allWarningOrErrorText += ("=======================================================\n");
                        allWarningOrErrorText += (target.platformName + " " + target.configurationName + "\n");
                        allWarningOrErrorText += ("Generated warning/errors, please fix and recompile.\n");
                        allWarningOrErrorText += (target.compileProcessErrorOutput);
                        allWarningOrErrorText += ("=======================================================\n\n");
                    }
                });

                if (allWarningOrErrorText.Count() > 0)
                {
                    warningOrErrorWindow.Show();
                    warningOrErrorWindow.TextBlock_WarningOrErrorsBox.Text += allWarningOrErrorText;
                    compileSucceeded = false;
                }
                else
                {
                    compileSucceeded = true;
                }
            }

            if (compileSucceeded)
            {
                OnCompilingSucceeded();
            }
            else
            {
                OnCompilingFailed();
            }

            Image_CompileResults.Visibility = Visibility.Visible;
        }

        private void OnCompilingSucceeded()
        {
            Image_CompileResults.Source = greenTickImage;
            Button_Commit.IsEnabled = true;
        }

        private void OnCompilingFailed()
        {
            Image_CompileResults.Source = redCrossImage;
            Button_Commit.IsEnabled = false;
        }

        private void OnCompileTimerTick(object source, EventArgs e)
        {
            bool haveAllProcessesFinished = !targetsToCompile.Any(target => target.IsProcessRunning() == true);
            if (haveAllProcessesFinished)
            {
                OnCompilingFinished();
                compileTimer.Stop();
            }
        }

        private void MainWindow_Button_Compile_OnClick(object sender, RoutedEventArgs e)
        {
            StartCompiling();
        }

        private void CommitChanges()
        {
            Button_Commit.Visibility = Visibility.Collapsed;
            Button_Committing.Visibility = Visibility.Visible;

            Process process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo.CreateNoWindow = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.UseShellExecute = false;

            process.StartInfo.FileName = "cmd.exe";
            process.StartInfo.Arguments = "/c CommitChecker-Windows-Commit.bat \"" + TextBox_CommitTitle.Text + "\" \"" + TextBox_CommitDescription.Text + "\"";

            bool processStartedSuccessfully = process.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to start the commit process. \nIs git installed? Is there a weird character in the title or description field?");

            process.Exited += CommitProcess_Exited;
        }

        private void CommitProcess_Exited(object sender, EventArgs e)
        {
            // The commit has gone through, close down everything now!
            Close();
        }

        private void MainWindow_Button_Commit_OnClick(object sender, RoutedEventArgs e)
        {
            CommitChanges();
        }

        private void MainWindow_Button_Close_OnClick(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
