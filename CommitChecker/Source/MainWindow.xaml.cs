﻿using CommitChecker.Source;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Media;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace CommitChecker
{
    public class TargetPlatformData
    {
        private string platformName = null;
        public string PlatformName
        {
            get { return platformName; }
            set { platformName = value; }
        }

        private string configurationName = null;
        public string ConfigurationName
        {
            get { return configurationName; }
            set { configurationName = value; }
        }

        private Process process = null;
        private string compilingScriptPath = null;
        private string compileProcessFullOutput;
        private string compileProcessErrorOutput;
        public string ErrorOutput
        {
            get { return compileProcessErrorOutput; }
        }

        public void SetupCompilingProcess()
        {
            FindCompilingScriptPathForPlatform();
            Debug.Assert((compilingScriptPath != null && compilingScriptPath.Count() > 0), "Compiling script for platform " + platformName + " & configuration " + configurationName + " doesn't exist?");
        
            // Windows can start compiling now
            // Linux will need file permissions and formatting first before calling StartCompilingProcess
            if (platformName == PlatformData.Windows)
            {
                StartCompilingProcess();
            }
        }

        public void StartCompilingProcess()
        {
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

        /// <summary>
        /// Used only for Linux
        /// Calls wsl chmod +x on the script file to give running permission
        /// </summary>
        private void GivePermissionForCompileScripts()
        {
            Debug.Assert(platformName == PlatformData.Linux, "Trying to give file permission on a Windows platform?");

            process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.FileName = "powershell.exe";
            process.StartInfo.Arguments = @"-executionpolicy unrestricted wsl chmod +x " + compilingScriptPath;
            process.StartInfo.WorkingDirectory = System.AppDomain.CurrentDomain.BaseDirectory;
            process.Exited += CompileScriptPermissionProcessFinished;

            bool processStartedSuccessfully = process.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to give chmod +x permission for " + platformName + ". \nPlease check the arguments!");
        }

        private void CompileScriptPermissionProcessFinished(object sender, EventArgs e)
        {
            FormatCompileScript();
        }

        /// <summary>
        /// Used only for Linux
        /// Runs wsl dos2unix script.sh to make sure scripts are always in the correct format for Linux
        /// </summary>
        private void FormatCompileScript()
        {
            Debug.Assert(platformName == PlatformData.Linux, "Trying to format script file on a Windows platform?");

            process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.FileName = "powershell.exe";
            process.StartInfo.Arguments = @"-executionpolicy unrestricted wsl dos2unix " + compilingScriptPath;
            process.StartInfo.WorkingDirectory = System.AppDomain.CurrentDomain.BaseDirectory;
            process.Exited += FormatCompileScriptProcessFinished;

            bool processStartedSuccessfully = process.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to convert script file using dos2unix for " + platformName + ". \nPlease check the arguments!");
        }

        private void FormatCompileScriptProcessFinished(object sender, EventArgs e)
        {
            StartCompilingProcess();
        }

        private void FindCompilingScriptPathForPlatform()
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
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Common
        private readonly Random random = new Random();

        // Window variables
        private readonly WarningOrErrorWindow warningOrErrorWindow = new WarningOrErrorWindow();
        private readonly TargetsWindow targetsWindow = new TargetsWindow();
        
        // Compiling variables
        private readonly List<TargetPlatformData> targetsToCompile = new List<TargetPlatformData>();
        private readonly DispatcherTimer compileTimer = new DispatcherTimer();
        private readonly BitmapImage greenTickImage = new BitmapImage(new Uri(@"Assets/Images/GreenTick.png", UriKind.Relative));
        private readonly BitmapImage redCrossImage = new BitmapImage(new Uri(@"Assets/Images/RedCross.png", UriKind.Relative));
        private bool compileSucceeded = false;

        // Compiling sounds
        private static MediaPlayer compileSuccessMediaPlayer = new MediaPlayer();
        private static MediaPlayer compileFailedMediaPlayer = new MediaPlayer();
        private readonly Uri compileSuccessSFX = new Uri(@"Assets/Audio/compile-success.wav", UriKind.Relative);
        private readonly List<Uri> compileFailedSFX = new List<Uri> 
        {
            new Uri(@"Assets/Audio/compile-fail1.mp3", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail2.flac", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail3.mp3", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail4.wav", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail5.wav", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail6.wav", UriKind.Relative),
            new Uri(@"Assets/Audio/compile-fail7.wav", UriKind.Relative),
        };

        // Commit variables
        private readonly Process commitProcess = new Process();
        private readonly DispatcherTimer commitTimer = new DispatcherTimer();
        private readonly DispatcherTimer delayAfterCommitHasGoneThroughTimer = new DispatcherTimer();
        private bool commitProcessSuccessful = false;
        private bool commitProcessFinished = false;

        public MainWindow()
        {
            //Debug.Listeners.Add(new AssertTraceListener());

            InitializeComponent();

            // Want to enable this platform/configuration by default
            AddTargetToCompile(PlatformData.Linux, PlatformData.Release);

            // Setup the timer here, but not starting it straight away, will be started once the user clicks on the "Compile" button.
            compileTimer.Tick += OnCompileTimerTick;
            compileTimer.Interval = TimeSpan.FromSeconds(1);

            commitTimer.Tick += CommitTimer_Tick;
            commitTimer.Interval = TimeSpan.FromSeconds(1);

            delayAfterCommitHasGoneThroughTimer.Tick += DelayAfterCommitHasGoneThroughTimer_Tick;
            delayAfterCommitHasGoneThroughTimer.Interval = TimeSpan.FromSeconds(2);

            commitProcess.EnableRaisingEvents = true;
            commitProcess.StartInfo.CreateNoWindow = true;
            commitProcess.StartInfo.RedirectStandardOutput = true;
            commitProcess.StartInfo.RedirectStandardError = true;
            commitProcess.StartInfo.UseShellExecute = false;

            commitProcess.StartInfo.FileName = "cmd.exe";
            commitProcess.Exited += CommitProcess_Exited;

            compileSuccessMediaPlayer.Open(compileSuccessSFX);
            compileFailedMediaPlayer.Open(GetRandomFailedSFX());

            StartCompiling();
        }

        Uri GetRandomFailedSFX()
        {
            int randomCompileFailedSFXIndex = random.Next(compileFailedSFX.Count);
            return compileFailedSFX[randomCompileFailedSFXIndex];
        }

        public void ClearTargetsToCompile()
        {
            targetsToCompile.Clear();
        }

        public void AddTargetToCompile(string platformName, string configuration)
        {
            TargetPlatformData platformData = new TargetPlatformData();
            platformData.PlatformName = platformName;
            platformData.ConfigurationName = configuration; 
            targetsToCompile.Add(platformData);
        }

        public void AddTargetToCompile(TargetPlatformData targetData)
        {
            targetsToCompile.Add(targetData);
        }

        public bool HasTargetToCompile(string platformName, string configurationName)
        {
            bool hasTarget = targetsToCompile.Any(target => (target.PlatformName == platformName && target.ConfigurationName == configurationName));
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

                // Clear any previous warnings/errors found and hide the window so it can pop up again when needed
                if (warningOrErrorWindow != null)
                {
                    warningOrErrorWindow.TextBlock_WarningOrErrorsBox.Text = "";
                    warningOrErrorWindow.Hide();
                }

                targetsToCompile.ForEach(target =>
                {
                    target.SetupCompilingProcess();
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
                        allWarningOrErrorText += (target.PlatformName + " " + target.ConfigurationName + "\n");
                        allWarningOrErrorText += ("Generated warning/errors, please fix and recompile.\n");
                        allWarningOrErrorText += (target.ErrorOutput);
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
            compileSuccessMediaPlayer.Play();
            compileSuccessMediaPlayer.MediaEnded += CompileSuccessMediaPlayer_MediaEnded;

            Image_CompileResults.Source = greenTickImage;
            Button_Commit.IsEnabled = true;
        }

        private void CompileSuccessMediaPlayer_MediaEnded(object sender, EventArgs e)
        {
            compileSuccessMediaPlayer.Position = TimeSpan.Zero;
            compileSuccessMediaPlayer.Stop();
        }

        private void OnCompilingFailed()
        {
            compileFailedMediaPlayer.Play();
            compileFailedMediaPlayer.MediaEnded += CompileFailedMediaPlayer_MediaEnded;

            Image_CompileResults.Source = redCrossImage;
            Button_Commit.IsEnabled = false;
        }

        private void CompileFailedMediaPlayer_MediaEnded(object sender, EventArgs e)
        {
            compileFailedMediaPlayer.Position = TimeSpan.Zero;
            compileFailedMediaPlayer.Stop();
            compileFailedMediaPlayer.Open(GetRandomFailedSFX());
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

            commitProcess.StartInfo.Arguments = "/c CommitChecker-Windows-Commit.bat \"" + TextBox_CommitTitle.Text + "\" \"" + TextBox_CommitDescription.Text + "\"";

            bool processStartedSuccessfully = commitProcess.Start();
            Debug.Assert(processStartedSuccessfully, "Failed to start the commit process. \nIs git installed? Is there a weird character in the title or description field?");

            commitTimer.Start();
        }

        private void CommitProcess_Exited(object sender, EventArgs e)
        {
            commitProcessSuccessful = (commitProcess.ExitCode == 0);
            commitProcessFinished = true;
        }

        private void CommitTimer_Tick(object sender, EventArgs e)
        {
            if (commitProcessFinished)
            {
                // Can stop this timer now
                commitTimer.Stop();

                // Display the commit button again to show that the "committing" is done and back to regular UI
                Button_Commit.Visibility = Visibility.Visible;
                Button_Committing.Visibility = Visibility.Collapsed;

                if (commitProcessSuccessful)
                {
                    // Disable the commit button, this app will be closing down in a couple of seconds
                    Button_Commit.IsEnabled = false;

                    // Green tick to let the user know their commit was successfully pushed to the repo.
                    Image_CommitResults.Source = greenTickImage;

                    // Can start the timer to close down the application now the commit process has finished.
                    delayAfterCommitHasGoneThroughTimer.Start();
                }
                else
                {
                    // Re-enable the commit button so the user can try again.
                    Button_Commit.IsEnabled = true;

                    // Something went wrong with the commit process?
                    // Perhaps the user needs to sign in or grant access through GitHub first?
                    // Or no internet connection? Not sure, either way, let them know _something_ went wrong at least...
                    Image_CommitResults.Source = redCrossImage;

                    // Reset the flag here because the user might need to try another commit since this one failed for whatever reason.
                    commitProcessFinished = false;
                }

                // Always show what the results were at this point.
                Image_CommitResults.Visibility = Visibility.Visible;
            }
        }

        private void MainWindow_Button_Commit_OnClick(object sender, RoutedEventArgs e)
        {
            CommitChanges();
        }

        private void DelayAfterCommitHasGoneThroughTimer_Tick(object sender, EventArgs e)
        {
            delayAfterCommitHasGoneThroughTimer.Stop();

            // The commit has gone through, close down everything now!
            Close();
            System.Windows.Application.Current.Shutdown();
        }

        private void MainWindow_Button_Close_OnClick(object sender, RoutedEventArgs e)
        {
            Close();
            System.Windows.Application.Current.Shutdown();
        }
    }
}
