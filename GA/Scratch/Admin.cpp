#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

// Member class
class Admin
{
private:
	string ad_username;
	string ad_password;
	string ad_fullname;
	string ad_phone;
public:
	// Constructor

	Admin(){}
	
	Admin(string ad_username, string ad_password, string ad_fullname, string ad_phone)
	: ad_username(ad_username), ad_password(ad_password), ad_fullname(ad_phone), ad_phone(ad_phone){}

	//Getter setter
		string getAdUsername()
	{
		return ad_username;
	}

	void setAdUsername(string ad_username)
	{
		ad_username = ad_username;
	}

	string getAdFullname()
	{
		return ad_fullname;
	}

	void setAdFullname(string ad_fullname)
	{
		ad_fullname = ad_fullname;
	}

	string getAdPhone()
	{
		return ad_phone;
	}

	void seAdtPhone(string ad_phone)
	{
		ad_phone = ad_phone;
	}

	string getAdPassword()
	{
		return ad_password;
	}

	void setAdPassword(string ad_password)
	{
		ad_password = ad_password;
	}

	/*-------------------------------------------------Function of Ad-------------------------------------------------*/
	
	//Admin Login function---------------------------------------------------------------------------------------------
	bool ReadAdminFile(string ad_username, string ad_password) {
    ifstream file("Data.txt");

    // Check if the file exists
    if (!file.good()) {
        cout << "Error: Unable to open file" << endl;
        exit(1);  // Terminate the program
    }

    string line;
    while (getline(file, line)) {  // Read a line of text from the file
        // Find the positions of the commas
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) {
            cout << "Error: Invalid login data format" << endl;
            return false;
        }

        // Extract the username and password from the line of text
        string username_from_file = line.substr(0, pos1);
        string password_from_file = line.substr(pos1 + 1, pos2 - pos1 - 1);

        // Compare the login information provided by the user to the data read from the file
        if (ad_username == username_from_file && ad_password == password_from_file) {
            return true;
        }
    }

    	return false;
	}

	void AdminLogin(){
		string ad_username, ad_password;
		bool check = false;

		while (true)
		{
			cout << "Enter your username: ";
			cin >> ad_username;

			cout << "Enter your password: ";
			cin >> ad_password;
			
			if (ReadAdminFile(ad_username, ad_password)) {
				cout << "Login successful!" << endl;
				adminMenu();
				break;
			} else {
				cout << "Invalid login information." << "\n";
				cout << "Please try again!" << "\n";
				cout << endl;

			}
		}	
	}

	//View all the all members and houses information-------------------------------------------------------------------------------
	void splitString(string input, string &username, string &pw, string &fullname, string &phone) {
		
		// Create a stringstream to split the input string
		stringstream ss(input);
		
		// Split the string by the comma delimiter
		getline(ss, username, ',');
		getline(ss, pw, ',');
		getline(ss, fullname, ',');
		getline(ss, phone, ',');
	}

	// Function to read and process the data in the file
	void readalldata() {
		// Open the file
		ifstream file("MemberData.txt");

		// Check if the file is open
		if (file.is_open()) {
			// Read in each line of the file
			string line;
			while (getline(file, line)) {
				// Declare variables to hold the data
				string username, pw, fullname, phone;
				
				// Split the string into the variables
				splitString(line, username, pw, fullname, phone);
				
				// Print the data to the console
				cout << "Username: " << username << endl;
				cout << "Password: " << pw << endl;
				cout << "Full Name: " << fullname << endl;
				cout << "Phone Number: " << phone << endl;
			}
			
			// Close the file
			file.close();
		}
		else {
			// Print an error message if the file cannot be opened
			cout << "Error opening file" << endl;
		}
	}

	//Admin Menu
	void adminMenu(){
		int choice;
		cout<< "\nThis is your menu:" << endl
			<< "0. Exit" << endl
			<< "1. View houses" << endl
			<< "Enter your choice: " << endl;
		cin >> choice;
	}
};
