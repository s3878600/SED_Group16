#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "Member.cpp"

using namespace std;

class System{
public:

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

};