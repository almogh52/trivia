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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaClient
{
    public struct RoomPreviewData
    {
        public string Name { get; set; }
        public int Id { get; set; }
        public int MaxPlayers { get; set; }
        public int TimePerQuestion { get; set; }
        public int QuestionCount { get; set; }
        public bool IsActive { get; set; }
        public bool IsJoinable { get; set; }
        public List<string> Players { get; set; }
    }

    /// <summary>
    /// Interaction logic for RoomPreview.xaml
    /// </summary>
    public partial class RoomPreview : UserControl
    {
        public RoomPreviewData Data { get; set; }

        public RoomPreview()
        {
            InitializeComponent();

            Update();
        }

        public void Update()
        {
            this.DataContext = Data;
        }
    }
}
