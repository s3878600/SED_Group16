#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void readData(string &username, string &password, string &fullname, string &phonenumber)
{
ifstream data("Data.txt"); // open the file
string line;
if (data.is_open())
{
    while (getline(data, line)) // read each line
    {
    // use stringstream to split the line by the comma delimiter
        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, fullname, ',');
        getline(ss, phonenumber, ',');
    // the remaining data is not needed
    }
    data.close(); // close the file
}
else
{
    cout << "Unable to open file" << endl;
}
}

int main()
{
    string username, password, fullname, phonenumber;
    readData(username, password, fullname, phonenumber);
    cout << "Username: " << username << endl;
    cout << "Password: " << password << endl;
    cout << "Full name: " << fullname << endl;
    cout << "Phone number: " << phonenumber << endl;
    return 0;
}