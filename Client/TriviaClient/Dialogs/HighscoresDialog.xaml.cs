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

namespace TriviaClient.Dialogs
{
    public struct HighscorePreview
    {
        public string Name { get; set; }
        public int Score { get; set; }
    }

    /// <summary>
    /// Interaction logic for HighscoresDialog.xaml
    /// </summary>
    public partial class HighscoresDialog : UserControl
    {
        public List<HighscorePreview> Highscores { get; set; }

        public HighscoresDialog()
        {
            Highscores = new List<HighscorePreview>();

            InitializeComponent();

            highscoresTable.DataContext = this;
        }
    }
}
