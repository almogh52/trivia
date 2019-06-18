using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient.Dialogs
{
    class CreateRoomDialogViewModel
    {
        public string Name { get; set; }
        public int MaxPlayers { get; set; }
        public int TimePerQuestion { get; set; }
        public int QuestionCount { get; set; }
    }
}
