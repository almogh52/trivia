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
using System.Windows.Shapes;

namespace TriviaClient
{
    public struct PlayerResults
    {
        public string username;
        public int correctAnswerCount;
        public int wrongAnswerCount;
        public int averageAnswerTime;
    }

    public struct GetGameResultsRequest
    {
        public const int CODE = 14;
    }
    
    public struct GetGameResultsResponse
    {
        public int status;
        public List<PlayerResults> results;
    }

    public struct PlayerResultsPreview
    {
        public string Username { get; set; }
        public int CorrectAnswers { get; set; }
        public int WrongAnswers { get; set; }
        public string AverageTime { get; set; }
    }

    /// <summary>
    /// Interaction logic for PlayerResultsWindow.xaml
    /// </summary>
    public partial class GameResultsWindow : Window
    {
        public List<PlayerResultsPreview> Results { get; set; }

        public GameResultsWindow()
        {
            InitializeComponent();
        }

        private async void RootDialog_Loaded(object sender, RoutedEventArgs e)
        {
            // Show the waiting for players dialog
            await DialogHost.Show(new Dialogs.WaitingForPlayersDialog(), "RootDialog", async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                GetGameResultsResponse res;

                // While we are still waiting for players
                while (true)
                {
                    byte[] buf;

                    try
                    {
                        // Send the get game results request
                        await Client.Send(GetGameResultsRequest.CODE, new byte[0]);

                        // Get from the server the response
                        buf = await Client.Recv();

                        // Deserialize the response
                        res = JsonConvert.DeserializeObject<GetGameResultsResponse>(Encoding.UTF8.GetString(buf));

                        // If got result SUCCESS, break the loop
                        if (res.status == 0)
                        {
                            break;
                        }

                        // Sleep 1 second
                        await Task.Delay(1000);
                    }
                    catch
                    {
                        Dispatcher.Invoke(() =>
                        {
                            // Close this window and re-show the auth window to connect to the server
                            new AuthWindow().Show();
                            Close();
                        });
                    }
                }

                // Parse the results
                ParseResults(res.results);

                // Close the dialog
                eventArgs.Session.Close();
            });

            // Set the data context of the results table
            Dispatcher.Invoke(() =>
            {
                resultsTable.DataContext = this;
            });
        }

        private void ParseResults(List<PlayerResults> results)
        {
            // Init list
            Results = new List<PlayerResultsPreview>();

            // For each player results
            foreach (PlayerResults playerResults in results)
            {
                // Convert the average time from ms to text
                TimeSpan t = TimeSpan.FromSeconds(playerResults.averageAnswerTime);
                string averageTime = string.Format("{0:D2}s",
                                        t.Seconds);

                Results.Add(new PlayerResultsPreview()
                {
                    Username = playerResults.username,
                    CorrectAnswers = playerResults.correctAnswerCount,
                    WrongAnswers = playerResults.wrongAnswerCount,
                    AverageTime = averageTime
                });
            }
        }

        private void returnToMenuBtn_Click(object sender, RoutedEventArgs e)
        {
            // Close this window and show the rooms window
            new RoomsWindow().Show();
            Close();
        }
    }
}
