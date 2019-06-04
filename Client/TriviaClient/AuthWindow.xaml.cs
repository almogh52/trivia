using MaterialDesignThemes.Wpf;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private async void RootDialog_Loaded(object sender, RoutedEventArgs e)
        {
            ConnectDialogViewModel connectDataContext = new ConnectDialogViewModel();

            // Show the connect dialog
            ConnectDialog dialog = new ConnectDialog();
            {
                DataContext = connectDataContext;
            };
            await DialogHost.Show(dialog, async delegate(object s1, DialogClosingEventArgs eventArgs)
            {
                string serverIP = connectDataContext.ServerIP;

                // Cancel the close of the dialog
                eventArgs.Cancel();

                // Set the content of the dialog to loading
                eventArgs.Session.UpdateContent(new LoadingDialog());

                try
                {
                    // Try to connect to the server
                    await Client.Connect(serverIP);

                    // If connected, close the dialog
                    eventArgs.Session.Close();
                } catch
                {
                    // Re-set the dialog as the connect dialog
                    eventArgs.Session.UpdateContent(dialog);
                }
            });
        }
    }
}
