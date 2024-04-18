using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace main.Classes
{
    public class Message
    {
        public Message(string messageName, int quantity, int userId, User user)
        {
            Id = MessagesCount;
            MessagesCount++;
            MessageName = messageName;
            Quantity = quantity;
            UserId = userId;
            User = user;
        }

        public static int MessagesCount = 0;
        public int Id { get; set; }
        public string MessageName { get; set; }
        public int Quantity { get; set; }
        public int UserId { get; set; }
        public User User { get; set; }

        public void Redact(string messageName, int quantity)
        {
            MessageName = messageName;
            Quantity = quantity;
        }

        public void PrintInfo()
        {
            Console.WriteLine("Id: " + Id + ", Message name : '" + MessageName + "', Quantity: " + Quantity + ", UserName: " + User.UserName +
                    ", User id: " + UserId + "\n--------------------------");
        }

    }
}
