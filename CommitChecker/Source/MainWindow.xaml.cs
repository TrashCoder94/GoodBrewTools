﻿using System;
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

            // TODO: Check the actual script file (.bat/.sh) and work out where it's best to update the working directory in there...
            // E.g. cd .. cd ..?
            // Might not work for the local test as I'd need to do that 3 times instead of 2 if it's put into the tools folder
            // Maybe I need to set the working directory via a command line?
            // Or here? Think.

            process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.UseShellExecute = false;

            // Windows - can just use the regulard start command to execute the build script.
            if (platformName == PlatformData.Windows)
            {
                process.StartInfo.FileName = "cmd.exe";
                process.StartInfo.Arguments = compilingScriptPath;
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

            //if (compileProcessFullOutput != null && compileProcessFullOutput.Count() > 0)
            //{
            //    Console.WriteLine("====================================");
            //    Console.WriteLine(platformName + " " + configurationName + " Output");
            //    Console.WriteLine(compileProcessFullOutput);
            //    Console.WriteLine("====================================");
            //}

            if (compileProcessErrorOutput != null && compileProcessErrorOutput.Count() > 0)
            {
                Console.WriteLine("====================================");
                Console.WriteLine(platformName + " " + configurationName + " Warning/Errors");
                Console.WriteLine(compileProcessErrorOutput);
                Console.WriteLine("====================================");
            }

            process.Close();
            process = null;
        }

        public bool IsProcessRunning()
        {
            return process != null;
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

                // Need to replace any backslashes with forward slashes
                if (platformName == PlatformData.Linux)
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
        private TargetsWindow targetsWindow = new TargetsWindow();
        private List<TargetPlatformData> targetsToCompile = new List<TargetPlatformData>();
        System.Windows.Threading.DispatcherTimer compileTimer = new System.Windows.Threading.DispatcherTimer();

        public MainWindow()
        {
            InitializeComponent();
            AddTargetToCompile(PlatformData.Linux, PlatformData.Debug);

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
            bool hasTarget = false;

            targetsToCompile.ForEach(target => 
            {
                if (hasTarget)
                {
                    return;
                }

                if(target.platformName == platformName && target.configurationName == configurationName)
                {
                    hasTarget = true;
                }
            });

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

        private void OnCompileTimerTick(object source, EventArgs e)
        {
            bool haveAllProcessesFinished = !targetsToCompile.Any(target => target.IsProcessRunning() == true);
            if (haveAllProcessesFinished)
            {
                Console.WriteLine("All compiler processes have finished now!");

                Button_Compile.Visibility = Visibility.Visible;
                Button_Compiling.Visibility = Visibility.Collapsed;
                compileTimer.Stop();
            }
            else
            {
                // Console.WriteLine("Still compiling...");
            }
        }

        private void MainWindow_Button_Compile_OnClick(object sender, RoutedEventArgs e)
        {
            if (targetsToCompile.Count > 0)
            {
                Button_Compile.Visibility = Visibility.Collapsed;
                Button_Compiling.Visibility = Visibility.Visible;

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
    }
}
