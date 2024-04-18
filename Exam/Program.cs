using main.Classes;

namespace main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            DataBase dataBase = new DataBase();

            dataBase.AddUser(new User("Олег"));
            dataBase.AddUser(new User("Надежда"));
            dataBase.AddUser(new User("Андрей"));

            dataBase.AddMessage(new Message("Привет, как дела?", 4, 0, dataBase.GetUserById(0)));
            dataBase.AddMessage(new Message("Всех с новым годом", 10, 0, dataBase.GetUserById(0)));
            dataBase.AddMessage(new Message("Спасибо", 1, 0, dataBase.GetUserById(0)));
            dataBase.AddMessage(new Message("Пожалуйста", 1, 0, dataBase.GetUserById(0)));
            dataBase.AddMessage(new Message("Не за что", 2, 0, dataBase.GetUserById(0)));

            dataBase.AddMessage(new Message("Люблю обнимаю", 1, 1, dataBase.GetUserById(1)));
            dataBase.AddMessage(new Message("Ты что себе позволяешь?", 2, 1, dataBase.GetUserById(1)));
            dataBase.AddMessage(new Message("Не за что", 1, 1, dataBase.GetUserById(1)));
            dataBase.AddMessage(new Message("Спасибо", 3, 1, dataBase.GetUserById(1)));
            dataBase.AddMessage(new Message("Ты где?", 8, 1, dataBase.GetUserById(1)));

            dataBase.AddMessage(new Message("Скоро буду", 3, 2, dataBase.GetUserById(2)));
            dataBase.AddMessage(new Message("Как вы там?", 5, 2, dataBase.GetUserById(2)));
            dataBase.AddMessage(new Message("Всё в силе?", 1, 2, dataBase.GetUserById(2)));
            dataBase.AddMessage(new Message("Ты покормила кошку?", 50, 2, dataBase.GetUserById(2)));
            dataBase.AddMessage(new Message("Я не знаю", 2, 2, dataBase.GetUserById(2)));

            MainMenu(ref dataBase);
        }

        static void MainMenu(ref DataBase dataBase)
        {
            while (true)
            {
                Console.Clear();

                Console.WriteLine("1. Пользователи\n2. Сообщения\n0. Выйти");
                int choice = Convert.ToInt32(Console.ReadLine());

                switch (choice)
                {
                    case 0:
                        return;
                    case 1:
                        UserMenu(ref dataBase);
                        break;
                    case 2:
                        MessageMenu(ref dataBase);
                        break;
                    default:
                        break;
                }
            }
        }

        static void UserMenu(ref DataBase dataBase)
        {
            while (true)
            {
                Console.Clear();

                Console.WriteLine("1. Все пользователи\n2. Все сообщения пользователя\n3. Добавалить пользователя\n4. Удалить пользователя\n5. Редактировать пользователя\n0. Назад");
                int choice = Convert.ToInt32(Console.ReadLine());

                Console.Clear();
                switch (choice)
                {
                    case 0:
                        return;
                    case 1:
                        dataBase.PrintUsersTable();
                        Console.ReadKey();
                        break;
                    case 2:
                        {
                            Console.Write("Enter id: ");
                            int id = Convert.ToInt32(Console.ReadLine());
                            dataBase.PrintUserMessages(id);
                            Console.ReadKey();
                            break;
                        }
                    case 3:
                        dataBase.CreateUser();
                        break;
                    case 4:
                        dataBase.DeleteUser();
                        break;
                    case 5:
                        dataBase.RedactUser();
                        break;
                    default:
                        break;
                }
            }
        }

        static void MessageMenu(ref DataBase dataBase)
        {
            while (true)
            {
                Console.Clear();

                Console.WriteLine("1. Все Сообщения\n2. Все сообщения пользователя\n3. Добавалить сообщение\n4. Удалить сообщение\n5. Редактировать сообщение\n0. Назад");
                int choice = Convert.ToInt32(Console.ReadLine());

                Console.Clear();
                switch (choice)
                {
                    case 0:
                        return;
                    case 1:
                        dataBase.PrintMessagesTable();
                        Console.ReadKey();
                        break;
                    case 2:
                        {
                            Console.Write("Enter id: ");
                            int id = Convert.ToInt32(Console.ReadLine());
                            dataBase.PrintUserMessages(id);
                            Console.ReadKey();
                            break;
                        }
                    case 3:
                        dataBase.CreateMessage();
                        break;
                    case 4:
                        dataBase.DeleteMessage();
                        break;
                    case 5:
                        dataBase.RedactMessage();
                        break;
                    default:
                        break;
                }
            }

        }
    }
}
