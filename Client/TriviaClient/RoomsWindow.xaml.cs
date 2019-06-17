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

    /// <summary>
    /// Interaction logic for RoomsWindow.xaml
    /// </summary>
    public partial class RoomsWindow : Window
    {
        public List<RoomData> Rooms { get; set; }

        public RoomsWindow()
        {
            InitializeComponent();
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

            // Send the get rooms request to the server
            await Client.Send(GetRoomsRequest.CODE, new byte[0]);

            // Get from the server the rooms available
            buf = await Client.Recv();

            // Deserialize the response
            res = JsonConvert.DeserializeObject<GetRoomsResponse>(Encoding.UTF8.GetString(buf));

            // Set the rooms
            Rooms = res.rooms == null ? new List<RoomData>() : res.rooms;
            roomsList.ItemsSource = Rooms;
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

                // Send the create room request to the server
                await Client.Send(CreateRoomRequest.CODE, Encoding.ASCII.GetBytes(JsonConvert.SerializeObject(createRoomRequest)));

                // Get the response from the server and deserialize it
                buf = await Client.Recv();
                createRoomResponse = JsonConvert.DeserializeObject<CreateRoomResponse>(Encoding.UTF8.GetString(buf));

                Debug.WriteLine(Encoding.UTF8.GetString(buf));

                // If the room creation failed, print message
                if (createRoomResponse.status == 1)
                {
                    // Prevent from the dialog to close
                    eventArgs.Cancel();

                    // Set the current dialog as an error dialog
                    eventArgs.Session.UpdateContent(new Dialogs.MessageDialog { Message = "An error occurred during creating the room!\nPlease try again later." });
                    return;
                }
            });

            // If no error occurred, add the room to the list of rooms
            if (createRoomResponse.status == 0)
            {
                Rooms.Add(new RoomData
                {
                    id = createRoomResponse.roomId,
                    name = viewModel.Name,
                    maxPlayers = viewModel.MaxPlayers,
                    timePerQuestion = viewModel.TimePerQuestion,
                    questionCount = viewModel.QuestionCount
                });

                // Re-set the list to apply changes
                roomsList.ItemsSource = Rooms;
            }
        }

        private async void refreshButton_Click(object sender, RoutedEventArgs e)
        {
            await UpdateRooms();
        }
    }
}
