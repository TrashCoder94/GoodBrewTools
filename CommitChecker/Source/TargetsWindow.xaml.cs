using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace CommitChecker
{
    /// <summary>
    /// Interaction logic for TargetsWindow.xaml
    /// </summary>
    public partial class TargetsWindow : Window
    {
        public TargetsWindow()
        {
            InitializeComponent();
        }

        private void TargetsWindow_Button_SaveAndClose_Click(object sender, RoutedEventArgs e)
        {
            // To the MainWindow somehow?
            var mainWindow = Application.Current.Windows.OfType<MainWindow>().FirstOrDefault();
            if (mainWindow != null)
            {
                mainWindow.ClearTargetsToCompile();

                // Linux Debug
                if(Checkbox_Targets_DebugLinux.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Linux, PlatformData.Debug);
                }

                // Linux Release
                if (Checkbox_Targets_ReleaseLinux.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Linux, PlatformData.Release);
                }

                // Linux Shipping
                if (Checkbox_Targets_ShippingLinux.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Linux, PlatformData.Shipping);
                }

                // Windows Debug
                if (Checkbox_Targets_DebugWindows.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Windows, PlatformData.Debug);
                }

                // Windows Release
                if (Checkbox_Targets_ReleaseWindows.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Windows, PlatformData.Release);
                }

                // Windows Shipping
                if (Checkbox_Targets_ShippingWindows.IsChecked.GetValueOrDefault(false))
                {
                    AddTargetToCompileToMainWindow(mainWindow, PlatformData.Windows, PlatformData.Shipping);
                }
            }

            Hide();
        }

        private void AddTargetToCompileToMainWindow(MainWindow mainWindow, string platformName, string configurationName)
        {
            TargetPlatformData newPlatformData = new TargetPlatformData();
            newPlatformData.PlatformName = platformName;
            newPlatformData.ConfigurationName = configurationName;
            mainWindow.AddTargetToCompile(newPlatformData);
        }

        private void TargetsWindow_TitleBar_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void OnShow()
        {
            var mainWindow = Application.Current.Windows.OfType<MainWindow>().FirstOrDefault();
            if (mainWindow != null)
            {
                // Update Linux checkboxes if they were previously ticked
                Checkbox_Targets_DebugLinux.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Linux, PlatformData.Debug);
                Checkbox_Targets_ReleaseLinux.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Linux, PlatformData.Release);
                Checkbox_Targets_ShippingLinux.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Linux, PlatformData.Shipping);

                // Do the same for the Windows checkboxes
                Checkbox_Targets_DebugWindows.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Windows, PlatformData.Debug);
                Checkbox_Targets_ReleaseWindows.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Windows, PlatformData.Release);
                Checkbox_Targets_ShippingWindows.IsChecked = mainWindow.HasTargetToCompile(PlatformData.Windows, PlatformData.Shipping);
            }
        }

        private void TargetsWindow_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if ((bool)e.NewValue)
            {
                OnShow();
            }
        }

        private void TargetWindow_Button_Close_OnClick(object sender, RoutedEventArgs e)
        {
            // Since this window can be shown/hidden whilst MainWindow is active.
            // So only Hide this window NOT close it.
            Hide();
        }
    }
}
