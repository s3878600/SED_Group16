#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "Member.cpp"

using namespace std;

const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 1800;

// System class
class System
{
public:
    vector<Member *> users;

    // Constructor
    System(){};

    //check if username is used already
    bool checkDupUsername(string username){
        bool isValid = false;
        for(Member *member: this->users){
            if(member->getUsername()==username) {
                isValid = true;
                cout<<"This username has been taken !"<<endl;
                break;
            }
        }
        return isValid;
    }

// Registration for non-member
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
        cout << "Enter Phone: ";
        string phone;
        cin >> phone;
        newMember->setPhone(phone);
        data += "," + phone;

       
        string enterHouse;
        while(enterHouse=="" || !isdigit(enterHouse[0]) || enterHouse.size()>1 || enterHouse != "1" && enterHouse!="2"){
             cout << "Enter house ? Press 1->Yes  2->No : ";
             cin >> enterHouse;
        }
        if (enterHouse == "1")
        {
            cout << "Enter Location: ";
            string location;
            cin >> location;
            data += "," + location;

            cout << "Enter Description: ";
            string description;
            cin >> description;
            data += "," + description;
            House *house = new House(location, description);
            newMember->setHouseForOwn(house);
        }
        
        ofstream myfile;
        myfile.open("Data.txt", fstream::app);
        myfile << data;

        return newMember;
    }

     // validate user choice
    bool checkChoice(string choice, string start, string end)
    {
        if ((choice.size() > 1) | (choice.size() < 1))
        {
            cout << "Invalid Input! Please enter your choice again" << endl
                 << "\n********************************************\n";
            return true;
        }
        else if ((start > choice | choice > end) | (!isdigit(choice[0])))
        {
            cout << "Invalid Input! Please enter your choice again" << endl
                 << "\n********************************************\n";
            return true;
        }
        return false;
    }

           // Split the spring by ','
    vector<string> splitString(string s)
    {
        vector<string> records;
        string add = "";
        s += ",";
        for (char ch : s)
        {
            if (ch == ',')
            {
                records.push_back(add);
                add = "";
                continue;
            }
            add += ch;
        }
        return records;

    }

    // Read all datas from file
    void readFile()
    {
        string output;
        ifstream myfile("Data.txt");
        vector<string> datas;
        while (getline(myfile, output))
        {
            // isAdmin, username, passsword, fullname, phone, location, description
            datas.push_back(output);
        }

        // create user and house to add to system
        for (string data : datas)
        {
            vector<string> splitData = splitString(data);

            Member *m = new Member();
            bool isAdmin = splitData[0] == "true" ? true : false;
            m->setIsAdmin(isAdmin);
            m->setUsername(splitData[1]);
            m->setPassword(splitData[2]);
            m->setFullname(splitData[3]);
            m->setPhone(splitData[4]);

            House *h = new House();
            h->setLocation(splitData[5]);
            h->setDescription(splitData[6]);

            m->setHouseForOwn(h);

            this->users.push_back(m);
        }
        myfile.close();
    }
};

