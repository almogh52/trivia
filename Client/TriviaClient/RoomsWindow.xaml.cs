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
    public struct Room
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public int MaxPlayers { get; set; }
        public int TimePerQuestion { get; set; }
        public bool IsActive { get; set; }
    }

    /// <summary>
    /// Interaction logic for RoomsWindow.xaml
    /// </summary>
    public partial class RoomsWindow : Window
    {
        public List<Room> Rooms { get; set; }

        public RoomsWindow()
        {
            Rooms = new List<Room>();

            Rooms.Add(new Room
            {
                Id = 1,
                Name = "sds"
            });
            Rooms.Add(new Room
            {
                Id = 2,
                Name = "Some Room"
            });

            InitializeComponent();

            roomsList.ItemsSource = Rooms;
        }
    }
}
