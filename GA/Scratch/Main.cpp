#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "Guest.cpp"
#include "Admin.cpp"

using namespace std;

// Main function start here

void option ()
{ 
    Guest *guest = new Guest();
    System *appSys = new System();
    Member *memBer = new Member();
    Admin *ad = new Admin();

    string choice;
    bool check = false;
    
    while (true)
    {
        cout<< "1. Use app as a guest   2. Login" << endl
            << "Enter your choice: " << endl;
        cin >> choice;
        if (!appSys->checkChoice(choice, "1", "2")) //Check valid input
        break;
    }

    // Choose one of the option
    if (choice == "1")
    {
        check = false;
        while(true)
        {
            cout << "\nRegist an account? Y/N" << endl;
            cin >> choice;

            if (choice == "Y" | choice == "y") // validate input
            {
                guest->registerAccount();
                cout << "\n" << endl;
                option(); //Return to menu
                break;
            }
            else if (choice == "N" | choice == "n"){
                guest->guestMenu();
                break;
            }   
            cout << "Invalid Input! Please enter your choice again\n";    
        }
    }
    
    else if (choice == "2") //Login as Member or Admin
    {
        while (true)
        {
            cout<< "Login as 1.Member 2.Admin" << endl
                << "Enter your choice: " << endl;
            cin >> choice;
            if (!appSys->checkChoice(choice, "1", "2")) //Check valid input
            break;
        }

        if (choice == "1") // validate input
        {
            cout << "\n" << endl;
        }
        else if (choice == "2"){
           ad->AdminLogin();
        }   
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