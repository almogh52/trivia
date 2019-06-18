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
    public partial class AuthWindow : Window
    {
        public bool Connected { get; set; }

        public AuthWindow()
        {
            InitializeComponent();

            loginForm.registerForm = registerForm;
            loginForm.showRoomsWindow = showRoomsWindow;

            registerForm.loginForm = loginForm;
            registerForm.showRoomsWindow = showRoomsWindow;
        }

        private void showRoomsWindow()
        {
            RoomsWindow roomsWindow = new RoomsWindow();

            // Close this window and the rooms window
            this.Close();
            roomsWindow.Show();
        }

        private async void RootDialog_Loaded(object sender, RoutedEventArgs e)
        {
            // If already connected, return
            if (Connected)
            {
                return;
            }

            Dialogs.ConnectDialogViewModel connectDataContext = new Dialogs.ConnectDialogViewModel();

            // Show the connect dialog
            Dialogs.ConnectDialog dialog = new Dialogs.ConnectDialog();
            {
                DataContext = connectDataContext;
            };
            await DialogHost.Show(dialog, async delegate(object s1, DialogClosingEventArgs eventArgs)
            {
                string serverIP = connectDataContext.ServerIP;

                // If already connected, return
                if (Connected)
                {
                    return;
                }

                // If the current content is an error message, re-set the connect dialog
                if (eventArgs.Session.Content.GetType() == typeof(Dialogs.MessageDialog))
                {
                    // Cancel the close of the dialog
                    eventArgs.Cancel();

                    // Set the connect dialog
                    eventArgs.Session.UpdateContent(dialog);
                    return;
                }

                // Cancel the close of the dialog
                eventArgs.Cancel();

                // Set the content of the dialog to loading
                eventArgs.Session.UpdateContent(new Dialogs.LoadingDialog());

                try
                {
                    // Try to connect to the server
                    await Client.Connect(serverIP);

                    // Set connected to true
                    Connected = true;

                    // If connected, close the dialog
                    eventArgs.Session.Close();
                } catch
                {
                    // Show an error message
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog() { Message = "Unable to connect to server, please validate the IP entered!" });
                }
            });
        }
    }
}
