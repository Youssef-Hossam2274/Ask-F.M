#include "Utility.cpp"
#include "Classes_Header.h"
#include <iostream>

void Run()
{
    Manager manager;
    vector<string> menu1, menu2;
    menu1.push_back("Login\n");
    menu1.push_back("Sign UP\n");
    menu2.push_back("Print Question To Me\n");
    menu2.push_back("Print Question From Me\n");
    menu2.push_back("Answer Question\n");
    menu2.push_back("Delete Question\n");
    menu2.push_back("Ask Question\n");
    menu2.push_back("List Syster Users\n");
    menu2.push_back("Feed\n");
    menu2.push_back("Logout\n");
    while (true)
    {
        manager.aboutMe();
        int choice = display(menu1, 1, 2);
        if (choice == 1)
            manager.Login();
        else if (choice == 2)
            manager.SignUp();

        while (true)
        {
            manager.aboutMe();
            choice = display(menu2, 1, 8);
            if (choice == 1)
                manager.PrintToMe();
            else if (choice == 2)
                manager.PrintFromMe();
            else if (choice == 3)
                manager.Aswer();
            else if (choice == 4)
                manager.Delete();
            else if (choice == 5)
                manager.Ask();
            else if (choice == 6)
                manager.List_Users();
            else if (choice == 7)
                manager.Feed();
            else if (choice == 8)
            {
                manager.logout();
                break;
            }
        }

    }
}

int main()
{
    Run();
    return 0;
}