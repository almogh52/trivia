using MaterialDesignThemes.Wpf;
using Newtonsoft.Json;
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
    public struct LoginRequest
    {
        public const int CODE = 0;

        public string username;
        public string password;
    }

    public struct LoginResponse
    {
        public int status;
    }

    /// <summary>
    /// Interaction logic for LoginForm.xaml
    /// </summary>
    public partial class LoginForm : UserControl
    {
        public RegisterForm registerForm { get; set; }
        public Action showRoomsWindow { get; set; }

        public LoginForm()
        {
            InitializeComponent();
        }

        private async void LoginBtn_Click(object sender, RoutedEventArgs e)
        {
            string username = usernameField.Text;
            string password = passwordField.Password;

            // Show error if the username or password is missing
            if (username.Length == 0 || password.Length == 0)
            {
                await DialogHost.Show(new Dialogs.MessageDialog { Message = "One of the required fields is missing" });

                return;
            }

            // Show the loading dialog
            await DialogHost.Show(new Dialogs.LoadingDialog(), async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                LoginResponse res;
                byte[] resBytes;

                LoginRequest req = new LoginRequest
                {
                    username = username,
                    password = password
                };

                // Send the login request
                await Client.Send(LoginRequest.CODE, Encoding.ASCII.GetBytes(JsonConvert.SerializeObject(req)));

                // Get the login response
                resBytes = await Client.Recv();

                // Deserialize the login response
                res = JsonConvert.DeserializeObject<LoginResponse>(Encoding.UTF8.GetString(resBytes));

                // If login failed, show error
                if (res.status == 1)
                {
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "Incorrect username and password entered!" });
                } else
                {
                    // Close the dialog
                    eventArgs.Session.Close();

                    // Show the rooms window
                    this.showRoomsWindow();
                }
            }, null);
        }

        private void RegisterBtn_Click(object sender, RoutedEventArgs e)
        {
            // Show the register from
            this.Opacity = 0;
            this.IsHitTestVisible = false;
            this.registerForm.Opacity = 1;
            this.registerForm.IsHitTestVisible = true;
        }
    }
}
