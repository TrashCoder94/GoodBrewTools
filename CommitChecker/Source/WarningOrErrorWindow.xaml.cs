﻿using System;
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
    /// Interaction logic for WarningOrErrorWindow.xaml
    /// </summary>
    public partial class WarningOrErrorWindow : Window
    {
        private ToolTip tooltip = new ToolTip();

        public WarningOrErrorWindow()
        {
            InitializeComponent();

            tooltip.Content = "Copied to clipboard!";
            tooltip.Foreground = Brushes.White;
            tooltip.Background = Brushes.Black;
        }

        private void WarningOrErrorWindow_Button_Close_Clicked(object sender, RoutedEventArgs e)
        {
            tooltip.IsOpen = false;
            Close();
        }

        private void WarningOrErrorsWindow_TitleBar_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void WarningOrErrorWindow_Button_CopyToClipboard_Clicked(object sender, RoutedEventArgs e)
        {
            Clipboard.SetText(TextBlock_WarningOrErrorsBox.Text);
            tooltip.IsOpen = true;
        }

        private void WarningOrErrorWindow_Button_CopyToClipboard_MouseLeave(object sender, MouseEventArgs e)
        {
            tooltip.IsOpen = false;
        }
    }
}
