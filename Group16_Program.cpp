#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "System.cpp"

// Main function start here
int main()
{
    System *appSys = new System();
    appSys->readFile();

    while (true)
    {
        bool check = false;
        string choice, role;
        Member *mem;
        while (true)
        {
            cout << "EEET2482/COSC2082 ASSIGNMENT" << endl
                 << "VACATION HOUSE EXCHANGE APPLICATION" << endl
                 << "\n"
                 << "Instructor: Mr. Linh Tran & Phong Ngo" << endl
                 << "Group: Group 16" << endl
                 << "s3911365, Vo Ngoc Diem Tien" << endl
                 << "s3911999, Nguyen Minh Thuan" << endl
                 << "s3878600, Vo Khanh Linh" << endl
                 << "s3751366, Cao Huy Tri" << endl
                 << "\n"
                 << "Use the app as 1. Guest   2. Member   3. Admin" << endl
                 << "Enter your choice: " << endl;
            cin >> role;
            if (!appSys->checkChoice(role, "1", "3")) // validate input
                break;
        }

        // Ask for registrartion for Guest
        if (role == "1")
        {
            check = false;
            while (true)
            {
                cout << "\nRegist an account? Y/N" << endl;
                cin >> choice;
                if (choice == "Y" | choice == "y") // validate input
                {
                    Member *mem1 = appSys->registerAccount();
                    appSys->users.push_back(mem1);
                    

                }
                else if (choice == "N" | choice == "n")
                    break;
                cout << "Invalid Input! Please enter your choice again\n";
            }
        }

        // Login with member or admin role
        else if (role == "2" || role == "3")
            mem = appSys->Login(appSys->users);
        while (choice != "0")
        {
            appSys->checkRole(role);
            cin >> choice;
            appSys->checkMenu(role, choice);
            appSys->checkFunction(role, choice, mem, appSys);
        }

        cout << "\nHave A Nice Day!!!" << endl
             << endl;
    }
    return 0;
}