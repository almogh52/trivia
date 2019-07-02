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
    public struct RoomData
    {
        public int id { get; set; }
        public string name { get; set; }
        public int maxPlayers { get; set; }
        public int timePerQuestion { get; set; }
        public int questionCount { get; set; }
        public bool isActive { get; set; }
    }

    public struct GetRoomsRequest
    {
        public const int CODE = 3;
    }

    public struct GetRoomsResponse
    {
        public int status;
        public List<RoomData> rooms;
    }

    public struct CreateRoomRequest
    {
        public const int CODE = 7;

        public string roomName;
        public int maxPlayers;
        public int questionCount;
        public int answerTimeout;
    }

    public struct CreateRoomResponse
    {
        public int status;
        public int roomId;
    }

    public struct LogoutRequest
    {
        public const int CODE = 2;
    }

    public struct LogoutResponse
    {
        public int status;
    }

    public struct GetPlayersInRoomRequest
    {
        public const int CODE = 4;

        public int roomId;
    }

    public struct GetPlayersInRoomResponse
    {
        public int status;
        public List<string> players;
    }

    public struct JoinRoomRequest
    {
        public const int CODE = 6;

        public int roomId;
    }

    public struct JoinRoomResponse
    {
        public int status;
    }

    public struct Highscore
    {
        public string name;
        public int score;
    }

    public struct HighscoreRequest
    {
        public const int CODE = 5;
    }

    public struct HighscoreResponse
    {
        public int status;
        public List<Highscore> highscores;
    }

    /// <summary>
    /// Interaction logic for RoomsWindow.xaml
    /// </summary>
    public partial class RoomsWindow : Window
    {
        public List<RoomData> Rooms { get; set; }

        public RoomsWindow()
        {
            InitializeComponent();

            roomPreview.Action = JoinRoom;
        }

        private async void JoinRoom(int roomId)
        {
            JoinRoomRequest req = new JoinRoomRequest
            {
                roomId = roomId
            };

            byte[] buf = null;
            JoinRoomResponse res;

            await DialogHost.Show(new Dialogs.LoadingDialog(), async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                // If already got a result
                if (buf != null)
                {
                    return;
                }

                // Send the join room request to the server
                await Client.Send(JoinRoomRequest.CODE, Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(req)));

                // Get the response from the server and deserialize it
                buf = await Client.Recv();
                res = JsonConvert.DeserializeObject<JoinRoomResponse>(Encoding.UTF8.GetString(buf));

                // If joined room successfully, show it
                if (res.status == 0)
                {
                    // Close the dialog
                    eventArgs.Session.Close();

                    // Close this window and show the room member window
                    new RoomMemberWindow
                    {
                        room = Rooms[roomsList.SelectedIndex]
                    }.Show();
                    Close();
                }
                else
                {
                    // Show the error dialog
                    eventArgs.Session.UpdateContent((new Dialogs.MessageDialog { Message = "Unable to join the wanted room!" }));
                }
            });
        }

        private async void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Update the rooms
            await UpdateRooms();
        }

        private async Task UpdateRooms()
        {
            GetRoomsResponse res;
            byte[] buf;

            try
            {
                // Send the get rooms request to the server
                await Client.Send(GetRoomsRequest.CODE, new byte[0]);

                // Get from the server the rooms available
                buf = await Client.Recv();

                // Deserialize the response
                res = JsonConvert.DeserializeObject<GetRoomsResponse>(Encoding.UTF8.GetString(buf));

                // Set the rooms
                Rooms = res.rooms == null ? new List<RoomData>() : res.rooms;
                roomsList.ItemsSource = Rooms;
            } catch
            {
                // Close this window and re-show the auth window to connect to the server
                new AuthWindow().Show();
                Close();
            }

        }

        private async void createButton_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomRequest createRoomRequest = new CreateRoomRequest();
            CreateRoomResponse createRoomResponse = new CreateRoomResponse()
            {
                status = -1
            };
            byte[] buf;

            Dialogs.CreateRoomDialogViewModel viewModel = new Dialogs.CreateRoomDialogViewModel()
            {
                MaxPlayers = -1,
                TimePerQuestion = -1,
                QuestionCount = -1
            };

            // Create the create room dialog using the room data as a data context
            Dialogs.CreateRoomDialog createRoomDialog = new Dialogs.CreateRoomDialog
            {
                DataContext = viewModel
            };

            // Show the create room dialog
            await DialogHost.Show(createRoomDialog, async delegate (object s1, DialogClosingEventArgs eventArgs)
            {
                // If the dialog is canceled ignore
                if (eventArgs.Parameter != null && (bool)eventArgs.Parameter == false) return;

                // If the create room failed, return
                if (createRoomResponse.status == 1)
                {
                    return;
                }

                // If the current content is an error message, re-set the create dialog
                if (eventArgs.Session.Content.GetType() == typeof(Dialogs.MessageDialog))
                {
                    // Cancel the close of the dialog
                    eventArgs.Cancel();

                    // Set the create dialog
                    eventArgs.Session.UpdateContent(createRoomDialog);
                    return;
                }

                // Check if the data entered is invalid
                if (viewModel.Name == null || viewModel.Name.Length == 0 || viewModel.MaxPlayers == -1 || viewModel.TimePerQuestion == -1 || viewModel.QuestionCount == -1)
                {
                    // Prevent from the dialog to close
                    eventArgs.Cancel();

                    // Set the current dialog as an error dialog
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "One or more of the values is empty or invalid!" });
                    return;
                }

                // Create the create room request using the user's values
                createRoomRequest.roomName = viewModel.Name;
                createRoomRequest.maxPlayers = viewModel.MaxPlayers;
                createRoomRequest.questionCount = viewModel.QuestionCount;
                createRoomRequest.answerTimeout = viewModel.TimePerQuestion;

                try
                {
                    // Send the create room request to the server
                    await Client.Send(CreateRoomRequest.CODE, Encoding.ASCII.GetBytes(JsonConvert.SerializeObject(createRoomRequest)));

                    // Get the response from the server and deserialize it
                    buf = await Client.Recv();
                    createRoomResponse = JsonConvert.DeserializeObject<CreateRoomResponse>(Encoding.UTF8.GetString(buf));

                    // If the room creation failed, print message
                    if (createRoomResponse.status == 1)
                    {
                        // Prevent from the dialog to close
                        eventArgs.Cancel();

                        // Set the current dialog as an error dialog
                        eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "An error occurred during creating the room!\nPlease try again later." });
                        return;
                    }
                    else
                    {
                        RoomData room = new RoomData
                        {
                            id = createRoomResponse.roomId,
                            name = viewModel.Name,
                            maxPlayers = viewModel.MaxPlayers,
                            questionCount = viewModel.QuestionCount,
                            timePerQuestion = viewModel.TimePerQuestion
                        };

                        // Show the room admin window
                        new RoomAdminWindow { room = room }.Show();

                        // Close this window
                        Close();
                    }
                } catch
                {
                    // Close this window and re-show the auth window to connect to the server
                    new AuthWindow().Show();
                    Close();
                }
            });
        }

        private async void refreshButton_Click(object sender, RoutedEventArgs e)
        {
            await UpdateRooms();
        }

        private async void logoutButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buf;
            LogoutResponse res;

            try
            {
                // Set the logout request to the server
                await Client.Send(LogoutRequest.CODE, new byte[0]);

                // Get the response from the server and deserialize it
                buf = await Client.Recv();
                res = JsonConvert.DeserializeObject<LogoutResponse>(Encoding.UTF8.GetString(buf));

                // Create an auth window and show it
                new AuthWindow()
                {
                    Connected = true
                }.Show();

                // Close the rooms window
                this.Close();
            } catch {
                // Close this window and re-show the auth window to connect to the server
                new AuthWindow().Show();
                Close();
            }
            
        }

        private async void roomsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (roomsList.SelectedItem == null)
            {
                // Set an empty data in the room preview
                roomPreview.Data = new RoomPreviewData();
                roomPreview.Update();

                return;
            }

            RoomData roomData = (RoomData)roomsList.SelectedItem;
            RoomPreviewData previewData = new RoomPreviewData
            {
                Name = roomData.name,
                Id = roomData.id,
                MaxPlayers = roomData.maxPlayers,
                TimePerQuestion = roomData.timePerQuestion,
                QuestionCount = roomData.questionCount,
                IsActive = roomData.isActive,
                ActionButtonText = "Join Room"
            };

            byte[] buf;
            GetPlayersInRoomRequest req = new GetPlayersInRoomRequest { roomId = roomData.id };
            GetPlayersInRoomResponse res;

            try
            {
                // Set the get players in room request to the server
                await Client.Send(GetPlayersInRoomRequest.CODE, Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(req)));

                // Get the response from the server and deserialize it
                buf = await Client.Recv();
                res = JsonConvert.DeserializeObject<GetPlayersInRoomResponse>(Encoding.UTF8.GetString(buf));

                // If an error occurred, show error
                if (res.status == 1)
                {
                    // Show dialog error
                    await DialogHost.Show(new Dialogs.MessageDialog { Message = "Unable to get the players in the requested roon" });
                }
                else
                {
                    // Set the players in the room
                    previewData.Players = res.players;

                    // Check if the room is joinable
                    previewData.ActionButtonEnabled = (previewData.IsActive && res.players.Count < previewData.MaxPlayers);

                    // Set the new data of the room preview
                    roomPreview.Data = previewData;
                    roomPreview.Update();
                }
            } catch
            {
                // Close this window and re-show the auth window to connect to the server
                new AuthWindow().Show();
                Close();
            }
            
        }

        private async void highscoresButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buf = null;
            HighscoreResponse res;

            Dialogs.HighscoresDialog dialog = new Dialogs.HighscoresDialog();

            await DialogHost.Show(new Dialogs.LoadingDialog(), async delegate (object s, DialogOpenedEventArgs eventArgs)
            {
                try
                {
                    // Send the join room request to the server
                    await Client.Send(HighscoreRequest.CODE, new byte[0]);

                    // Get the response from the server and deserialize it
                    buf = await Client.Recv();
                    res = JsonConvert.DeserializeObject<HighscoreResponse>(Encoding.UTF8.GetString(buf));

                    // If joined room successfully, show it
                    if (res.status == 0)
                    {
                        if (res.highscores != null)
                        {
                            // For each highscore create a highscore preview
                            foreach (Highscore score in res.highscores)
                            {
                                dialog.Highscores.Add(new Dialogs.HighscorePreview { Name = score.name, Score = score.score });
                            }
                        }

                        // Show the highscores dialog
                        eventArgs.Session.UpdateContent(dialog);
                    }
                    else
                    {
                        // Show the error dialog
                        eventArgs.Session.UpdateContent((new Dialogs.MessageDialog { Message = "Unable to get the highscores table!" }));
                    }
                } catch
                {
                    // Close this window and re-show the auth window to connect to the server
                    new AuthWindow().Show();
                    Close();
                }
            });
        }
    }
}
