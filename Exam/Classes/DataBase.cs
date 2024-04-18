using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace main.Classes
{
    public class DataBase
    {

        private List<User> Users;
        private List<Message> Messages;
        public DataBase()
        {
            Users = new List<User>();
            Messages = new List<Message>();
        }

        public void AddUser(User user)
        {
            Users.Add(user);
        }
        public void RemoveUser(User user)
        {
            Users.Remove(user);
        }

        public User GetUserById(int id)
        {
            return Users.First(x => x.Id == id);
        }

        public void AddMessage(Message message)
        {
            Messages.Add(message);
            Users.First(x => x.Id == message.UserId).Messages.Add(message);
        }
        public void RemoveMessage(Message message)
        {
            Users.First(x => x.Id == message.UserId).Messages.Remove(message);
            Messages.Remove(message);
        }

        public void PrintUsersTable()
        {
            foreach (User user in Users)
            {
                user.PrintInfo();
            }
        }
        public void PrintMessagesTable()
        {
            foreach (Message message in Messages)
            {
                message.PrintInfo();
            }
        }

        public void PrintUserMessages(int userId)
        {
            User? currentUser = Users.FirstOrDefault(x => x.Id == userId);
            if (currentUser == null)
            {
                Console.WriteLine("User not found.");
                Console.ReadKey();
                return;
            }

            foreach (Message message in currentUser.Messages)
            {
                message.PrintInfo();
            }
        }

        public void CreateUser()
        {
            Console.Write("Enter UserName: ");
            string? name = Console.ReadLine();

            if (name == null)
            {
                Console.WriteLine("Empty username.");
                Console.ReadKey();
                return;
            }

            Users.Add(new User(name));
        }

        public void CreateMessage()
        {
            Console.Write("Enter user id: ");
            int id = Convert.ToInt32(Console.ReadLine());

            User? user = Users.FirstOrDefault(x => x.Id == id);
            if (user == null)
            {
                Console.WriteLine("Not found.");
                Console.ReadKey();
                return;
            }

            Console.Write("Enter message name: ");
            string? messageName = Console.ReadLine();
            if (messageName == null)
            {
                Console.WriteLine("Empty message.");
                Console.ReadKey();
                return;
            }

            Console.Write("Enter quantity: ");
            int quantity = Convert.ToInt32(Console.ReadLine());
            Message message = new Message(messageName, quantity, user.Id, user);
            Messages.Add(message);
            user.Messages.Add(message);
        }

        public void DeleteUser()
        {
            Console.Write("Enter user id: ");
            int userId = Convert.ToInt32(Console.ReadLine());
            User? user = Users.FirstOrDefault(x => x.Id == userId);
            if (user == null)
            {
                Console.WriteLine("Not found.");
                Console.ReadKey();
                return;
            }
            RemoveUser(user);
        }
        public void DeleteMessage()
        {
            Console.Write("Enter message id: ");
            int messageId = Convert.ToInt32(Console.ReadLine());
            Message? message = Messages.FirstOrDefault(x => x.Id == messageId);
            if (message == null)
            {
                Console.WriteLine("Not found.");
                Console.ReadKey();
                return;
            }
            RemoveMessage(message);
        }

        public void RedactUser()
        {
            Console.Write("Enter user id: ");
            int id = Convert.ToInt32(Console.ReadLine());

            User? user = Users.FirstOrDefault(x => x.Id == id);
            if (user == null)
            {
                Console.WriteLine("Not found.");
                Console.ReadKey();
                return;
            }

            Console.Write("Enter new name: ");
            string? name = Console.ReadLine();
            if (name == null)
            {
                Console.WriteLine("Empty.");
                Console.ReadKey();
                return;
            }
            user.Redact(name);
        }

        public void RedactMessage()
        {
            Console.Write("Enter message id: ");
            int id = Convert.ToInt32(Console.ReadLine());

            Message? message = Messages.FirstOrDefault(x => x.Id == id);
            if (message == null)
            {
                Console.WriteLine("Not found.");
                Console.ReadKey();
                return;
            }

            Console.Write("Enter new name: ");
            string? name = Console.ReadLine();
            if (name == null)
            {
                Console.WriteLine("Empty.");
                Console.ReadKey();
                return;
            }

            Console.Write("Enter new quantity: ");
            int quantity = Convert.ToInt32(Console.ReadLine());

            message.Redact(name, quantity);

            User? user = Users.FirstOrDefault(x => message.UserId == id);
            if (user == null)
            {
                return;
            }
            user.Messages.First(x => x.Id == id).Redact(name, quantity);
        }

    }
}
