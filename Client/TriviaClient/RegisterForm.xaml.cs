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
    public struct RegisterRequest
    {
        public const int CODE = 1;

        public string username;
        public string password;
        public string email;
    }

    public struct RegisterResponse
    {
        public int status;
    }

    /// <summary>
    /// Interaction logic for LoginForm.xaml
    /// </summary>
    public partial class RegisterForm : UserControl
    {
        public LoginForm loginForm { get; set; }

        public RegisterForm()
        {
            InitializeComponent();
        }

        private async void RegisterBtn_Click(object sender, RoutedEventArgs e)
        {
            string username = usernameField.Text;
            string password = passwordField.Password;
            string email = emailField.Text;

            // Show error if the username or password or email is missing
            if (username.Length == 0 || password.Length == 0 || email.Length == 0)
            {
                await DialogHost.Show(new Dialogs.MessageDialog { Message = "One of the required fields is missing" });

                return;
            }

            // Show the loading dialog
            await DialogHost.Show(new Dialogs.LoadingDialog(), async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                RegisterResponse res;
                byte[] resBytes;

                RegisterRequest req = new RegisterRequest
                {
                    username = username,
                    password = password,
                    email = email
                };

                // Send the register request
                await Client.Send(RegisterRequest.CODE, Encoding.ASCII.GetBytes(JsonConvert.SerializeObject(req)));

                // Get the register response
                resBytes = await Client.Recv();

                // Deserialize the register response
                res = JsonConvert.DeserializeObject<RegisterResponse>(Encoding.UTF8.GetString(resBytes));

                // If register failed, show error
                if (res.status == 1)
                {
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "The username is already in use!" });
                }
            }, null);
        }

        private void LoginBtn_Click(object sender, RoutedEventArgs e)
        {
            // Show the login form
            this.Opacity = 0;
            this.IsHitTestVisible = false;
            this.loginForm.Opacity = 1;
            this.loginForm.IsHitTestVisible = true;
        }
    }
}
