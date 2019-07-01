using MaterialDesignThemes.Wpf;
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

    public struct LeaveGameRequest
    {
        public const int CODE = 15;
    }

    public struct SubmitAnswerRequest
    {
        public const int CODE = 13;

        public int answerId;
    }

    public struct SubmitAnswerResponse
    {
        public int status;
        public int correctAnswerId;
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
        private RadioButton[] radioButtons { get; set; }

        public GameWindow()
        {
            SelectedAnswer = -1;

            InitializeComponent();

            radioButtons = new[] { radioBtn0, radioBtn1, radioBtn2, radioBtn3 };
        }

        private void Update()
        {
            DataContext = null;
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
            await GetQuestion();

            Update();
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton btn = sender as RadioButton;

            // Set the new selected answer and update
            SelectedAnswer = int.Parse((string)btn.Tag);
        }

        private async void leaveGameBtn_Click(object sender, RoutedEventArgs e)
        {
            byte[] buf;

            // Send the leave game request
            await Client.Send(LeaveGameRequest.CODE, new byte[0]);

            // Get from the server the response
            buf = await Client.Recv();

            // Show the rooms window
            new RoomsWindow().Show();
            Close();
        }

        private RadioButton GetRadioButtonForAnswer(int answerId)
        {
            int realIndex = Array.IndexOf(CurrentQuestion.AnswerIndices, answerId);

            return radioButtons[realIndex];
        }

        private async Task SubmitAnswer()
        {
            SubmitAnswerRequest req = new SubmitAnswerRequest
            {
                answerId = CurrentQuestion.AnswerIndices[SelectedAnswer]
            };
            SubmitAnswerResponse res;
            byte[] buf;

            int[] answerIndices = RandomAnswerIndices();

            // Send the submit answer request
            await Client.Send(SubmitAnswerRequest.CODE, Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(req)));

            // Get from the server the response
            buf = await Client.Recv();

            // Deserialize the response
            res = JsonConvert.DeserializeObject<SubmitAnswerResponse>(Encoding.UTF8.GetString(buf));

            // If an error occurred
            if (res.status == 1)
            {
                // Show error
                await DialogHost.Show(new Dialogs.MessageDialog { Message = "Unable to submit answer!" });

                throw new Exception();
            } else
            {
                // Highlight the correct answer
                GetRadioButtonForAnswer(res.correctAnswerId).Foreground = Brushes.LightGreen;

                // If was wrong, highlight in red the wrong answer
                if (res.correctAnswerId != CurrentQuestion.AnswerIndices[SelectedAnswer])
                {
                    radioButtons[SelectedAnswer].Foreground = Brushes.Red;
                }
            }
        }

        private void UnselectRadioButtons()
        {
            SelectedAnswer = -1;

            // Unselect all buttons
            foreach (RadioButton btn in radioButtons)
            {
                btn.IsChecked = false;
                btn.Foreground = Brushes.White;
            }
        }

        private void DisableRadioButtons()
        {
            // Disable all buttons
            foreach (RadioButton btn in radioButtons)
            {
                btn.IsHitTestVisible = false;
            }
        }

        private void EnableRadioButtons()
        {
            // Enable all buttons
            foreach (RadioButton btn in radioButtons)
            {
                btn.IsHitTestVisible = true;
            }
        }

        private async void submitNextBtn_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;

            // If nothing selected, return
            if (SelectedAnswer == -1)
            {
                return;
            }

            // Disable the button
            btn.IsEnabled = false;

            // Submit
            if ((string)btn.Content == "Submit")
            {
                DisableRadioButtons();

                try
                {
                    await SubmitAnswer();
                } catch {
                    // Enable the button
                    btn.IsEnabled = true;

                    EnableRadioButtons();

                    return;
                }

                // Enable the button
                btn.IsEnabled = true;

                // Change the button to the next button
                btn.Content = "Next";
            } else
            {
                UnselectRadioButtons();

                // Get the next question
                await GetQuestion();
                Update();

                // Enable the button
                btn.IsEnabled = true;

                // Change the button to the next button
                btn.Content = "Submit";

                EnableRadioButtons();
            }
        }
    }
}
