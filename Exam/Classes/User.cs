using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace main.Classes
{
    public class User
    {
        public User(string userName)
        {
            Id = UserCount;
            UserCount++;
            UserName = userName;
            Messages = new List<Message>();
        }
        static private int UserCount = 0;
        public int Id { get; set; }
        public string UserName { get; set; }
        public List<Message> Messages { get; set; }

        public void Redact(string userName)
        {
            UserName = userName;
        }

        public void PrintInfo()
        {
            int messageQuantity = 0;
            foreach (Message message in Messages)
            {
                messageQuantity += message.Quantity;
            }

            Console.WriteLine("Id: " + Id + ", Name: " + UserName + ", Message quantity: " + messageQuantity + "\n--------------------------");
        }
    }
}
