#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "System.cpp"


void option()
{
    while(true)
    {
        System *appSys = new System();
        appSys->readFile();
        bool check = false;
        string choice, role;
        Member *mem;
        
        while (true)
        {
            cout<<"-----------------------WELCOME---------------------"<< endl
                << "This is your menu: " << endl
                << "1. Guest  ||  2. Login" << endl
                << "Enter your choice: " << endl;
            cin >> choice;
            if (!appSys->checkChoice(choice, "1", "2")) //Check valid input
            break;
        }


        if (choice == "1")
        {
            role = "1"; 
            check = false;
            while(true)
            {
                cout << "\nDo you want to register?  Y/N" << endl;
                cin >> choice;

                if (choice == "Y" | choice == "y") 
                {
                    appSys->regisNewAccount();
                    cout << "\n" << endl;
                    option(); 
                    break;
                }
                else if (choice == "N" | choice == "n"){
                    break;
                }   
                cout << "Wrong input! Please type Y or N only\n";    
            }
        }
        

        else if (choice == "2") 
        {
            while (true)
            {
                cout<< "1.Member login || 2.Admin login" << endl
                    << "Enter your choice: " << endl;
                cin >> choice;
                if (!appSys->checkChoice(choice, "1", "2")) 
                break;
            }

            if (choice == "1") 
            {
                role = "2"; 
                mem = appSys->Login(appSys->users);
            }
            else if (choice == "2"){
                role = "3";
                mem = appSys->Login(appSys->users);
            }   
        }

        while (choice != "0")
        {
            appSys->confirmRole(role);
            cin >> choice;
            appSys->confirmMenu(role, choice);
            appSys->confirmFunction(role, choice, mem, appSys);
        }
        cout << "\n You have been logged out " << endl;
    }
}


int main()
{
    cout<< "EEET2482/COSC2082 ASSIGNMENT" << endl
        << "VACATION HOUSE EXCHANGE APPLICATION" << endl
        << "\n"
        << "Instructor: Mr. Linh Tran & Phong Ngo" << endl
        << "Group: Group 16" << endl
        << "s3911365, Vo Ngoc Diem Tien" << endl
        << "s3911999, Nguyen Minh Thuan" << endl
        << "s3878600, Vo Khanh Linh" << endl
        << "s3751366, Cao Huy Tri" << endl
        << "\n";

    option();
    return 0;
}