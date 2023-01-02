#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>

#include "System.cpp"

using namespace std;

class Guest{
public:
    vector<Member *> users;
    System *appSys = new System();

    // Constructor
    Guest(){};

    //check if username is used already
    bool checkDupUsername(string username){
        bool isValid = false;
        for(Member *member: users){
            if(member->getUsername()==username) {
                isValid = true;
                cout<<"This username has been taken !"<<endl;
                break;
            }
        }
        return isValid;
    }
    
    /*----------------------------------Registration for non-member---------------------------------*/
        
    //Check phone input type
    string getValidPhoneNumber() {
        string phoneNumber;
        bool isValid = false;

        while (!isValid) {
            cout << "Enter Phone: ";
            cin >> phoneNumber;

            if (phoneNumber.length() == 10 && phoneNumber[0] == '0') {
                isValid = true;
            } else {
                cout << "Invalid phone number. Please try again." << endl;
            }
        }
        return phoneNumber;
    }
    
    Member *registerAccount()
    {
        cout << "-----Register account--------\n";
        string data = "";
        
        Member *newMember = new Member();

        string username;
        while(username=="" ||checkDupUsername(username)) {
            cout << "Enter username: ";
            cin >> username;
        }
        newMember->setUsername(username);
        data += username;

    
        //Passwword
        cout << "Enter password: ";
        string password;
        cin >> password;
        newMember->setPassword(password);
        data += "," + password;

        //Fullname
        cout << "Enter fullname: ";
        string fullname;
        cin >> fullname;
        newMember->setFullname(fullname);
        data += "," + fullname;

        //Phone
         //Enter phone number (must be 10 digits and start with 0)
        string phone = getValidPhoneNumber(); 
        newMember->setPhone(phone);
        data += "," + phone;
        
        ofstream myfile;
        myfile.open("MemberData.txt", fstream::app);
        myfile << data << endl;
        cout << "You have successfully registered account";

        return newMember;
    }

    void guestMenu(){
        int choice;
        cout<< "\nThis is your menu:" << endl
            << "0. Exit" << endl
            << "1. View houses" << endl
            << "Enter your choice: " << endl;
        cin >> choice;
        
        if(choice == 0){
        }
    }

    friend System;
};
