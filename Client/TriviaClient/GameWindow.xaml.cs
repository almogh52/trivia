using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
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
    public struct GetQuestionRequest
    {
        public const int CODE = 12;
    }

    public struct GetQuestionResponse
    {
        public int status;
        public string question;
        public SortedDictionary<int, string> answers;
    }

    public struct QuestionPreview
    {
        public string Question { get; set; }
        public List<string> Answers { get; set; }
        public int[] AnswerIndices { get; set; }
    }

    /// <summary>
    /// Interaction logic for GameWindow.xaml
    /// </summary>
    public partial class GameWindow : Window
    {
        public QuestionPreview CurrentQuestion { get; set; }
        public int SelectedAnswer { get; set; }

        private Random rnd = new Random();

        public GameWindow()
        {
            SelectedAnswer = -1;

            InitializeComponent();
        }

        private void Update()
        {
            DataContext = this;
        }

        private int[] RandomAnswerIndices()
        {
            int[] indices = { 0, 1, 2, 3 };

            return indices.OrderBy(x => rnd.Next()).ToArray();
        }

        private List<string> GetAnswersByIndices(SortedDictionary<int, string> answers, int[] answerIndices)
        {
            List<string> answersList = new List<string>();

            // For each answer index, add the answer to the list of answers
            foreach (int answerIdx in answerIndices)
            {
                answersList.Add(answers[answerIdx]);
            }

            return answersList;
        }

        private async Task GetQuestion()
        {
            GetQuestionResponse res;
            byte[] buf;

            int[] answerIndices = RandomAnswerIndices();

            // Send the get question request
            await Client.Send(GetQuestionRequest.CODE, new byte[0]);

            // Get from the server the response
            buf = await Client.Recv();

            // Deserialize the response
            res = JsonConvert.DeserializeObject<GetQuestionResponse>(Encoding.UTF8.GetString(buf));

            // Set the current question
            CurrentQuestion = new QuestionPreview
            {
                Question = res.question,
                AnswerIndices = answerIndices,
                Answers = GetAnswersByIndices(res.answers, answerIndices)
            };
        }

        private async void Window_Loaded(object sender, RoutedEventArgs e)
        {
            await Task.Run(GetQuestion);

            Update();
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton btn = sender as RadioButton;

            // Set the new selected answer and update
            SelectedAnswer = int.Parse((string)btn.Tag);
        }
    }
}
