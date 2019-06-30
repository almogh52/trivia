using Newtonsoft.Json;
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

    public struct LeaveRoomRequest
    {
        public const int CODE = 11;
    }

    public struct LeaveRoomResponse
    {
        public int status;
    }

    /// <summary>
    /// Interaction logic for RoomMemberWindow.xaml
    /// </summary>
    public partial class RoomMemberWindow : Window
    {
        public RoomData room { get; set; }

        public RoomMemberWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Set title
            Title = $"Trivia - {room.name} - Room #{room.id}";

            // Set the action handler of the room preview
            roomPreview.Action = LeaveRoom;

            // Run the action in the background
            Task.Run(new Action(RefreshRoomData));
        }

        private async void RefreshRoomData()
        {
            GetRoomStateResponse res;
            byte[] buf;

            while (true)
            {
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
                    IsActive = res.hasGameBegun,
                    ActionButtonText = "Leave Room",
                    ActionButtonEnabled = true,
                    Players = res.players
                };

                // Update the room preview in the main thread
                roomPreview.Dispatcher.Invoke(new Action(() => { roomPreview.Update(); }));

                // Wait 1 second
                await Task.Delay(1000);
            }
        }

        private async void LeaveRoom(int roomId)
        {
            LeaveRoomResponse res;
            byte[] buf;

            // Send the leave room request
            await Client.Send(LeaveRoomRequest.CODE, new byte[0]);

            // Get from the server the response
            buf = await Client.Recv();

            // Deserialize the response
            res = JsonConvert.DeserializeObject<LeaveRoomResponse>(Encoding.UTF8.GetString(buf));

            // Close this window and the rooms window
            new RoomsWindow().Show();
            Close();
        }
    }
}
