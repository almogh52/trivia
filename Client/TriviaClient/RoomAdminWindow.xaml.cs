using MaterialDesignThemes.Wpf;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    public struct GetRoomStateRequest
    {
        public const int CODE = 10;
    }

    public struct GetRoomStateResponse
    {
        public int status;
        public bool hasGameBegun;
        public List<string> players;
        public int questionCount;
        public int answerTimeout;
    }

    public struct CloseRoomRequest
    {
        public const int CODE = 8;
    }

    public struct CloseRoomResponse
    {
        public int status;
    }

    public struct StartGameRequest
    {
        public const int CODE = 9;
    }

    public struct StartGameResponse
    {
        public int status;
    }

    /// <summary>
    /// Interaction logic for RoomAdminWindow.xaml
    /// </summary>
    public partial class RoomAdminWindow : Window
    {
        public RoomData room { get; set; }

        private CancellationTokenSource tokenSource = new CancellationTokenSource();

        public RoomAdminWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Set title
            Title = $"Trivia - {room.name} - Room #{room.id}";

            // Set the action handler of the room preview
            roomPreview.Action = CloseRoom;

            // Run the action in the background
            Task.Run(new Action(RefreshRoomData));
        }

        private async void RefreshRoomData()
        {
            GetRoomStateResponse res;
            byte[] buf;

            while (true)
            {
                // Exit if requested
                if (tokenSource.IsCancellationRequested) return;
      
                // Send the get room state request
                await Client.Send(GetRoomStateRequest.CODE, new byte[0]);

                // Get from the server the room state
                buf = await Client.Recv();

                // Deserialize the response
                res = JsonConvert.DeserializeObject<GetRoomStateResponse>(Encoding.UTF8.GetString(buf));

                // Set the room preview data
                roomPreview.Data = new RoomPreviewData
                {
                    Id = room.id,
                    Name = room.name,
                    MaxPlayers = room.maxPlayers,
                    TimePerQuestion = res.answerTimeout,
                    QuestionCount = res.questionCount,
                    IsActive = !res.hasGameBegun,
                    ActionButtonText = "Close Room",
                    ActionButtonEnabled = true,
                    Players = res.players
                };

                // Update the room preview in the main thread
                roomPreview.Dispatcher.Invoke(new Action(() => { roomPreview.Update(); }));

                // Wait 1 second
                await Task.Delay(1000);
            }
        }

        private async void CloseRoom(int roomId)
        {
            CloseRoomResponse res;
            byte[] buf;

            // Send the close room request
            await Client.Send(CloseRoomRequest.CODE, new byte[0]);

            // Get from the server the response
            buf = await Client.Recv();

            // Deserialize the response
            res = JsonConvert.DeserializeObject<CloseRoomResponse>(Encoding.UTF8.GetString(buf));

            // Cancel the background action
            tokenSource.Cancel();

            // Close this window and the rooms window
            new RoomsWindow().Show();
            Close();
        }

        private async void startGameButton_Click(object sender, RoutedEventArgs e)
        {
            // Cancel the background action
            tokenSource.Cancel();

            // Show the loading dialog
            await DialogHost.Show(new Dialogs.LoadingDialog(), async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                StartGameResponse res;
                byte[] buf;

                // Send the start game request
                await Client.Send(StartGameRequest.CODE, new byte[0]);

                // Get from the server the response
                buf = await Client.Recv();

                // Deserialize the response
                res = JsonConvert.DeserializeObject<StartGameResponse>(Encoding.UTF8.GetString(buf));

                // If an error occurred
                if (res.status == 1)
                {
                    // Show an error dialog
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "Unable to start the game!" });

                    // Start the background action again
                    Dispatcher.Invoke(new Action(() =>
                    {
                        Task.Run(new Action(RefreshRoomData));
                    }));
                } else
                {
                    Dispatcher.Invoke(new Action(() =>
                    {
                        // Show the game window
                        new GameWindow().Show();
                        Close();
                    }));
                }
            });
        }
    }
}
