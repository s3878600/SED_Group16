#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

#include "Member.cpp"

using namespace std;

class System
{
public:
    vector<Member *> users;
    System(){};


    Member *getMemberByUsername(string username)
    {
        Member *res;
        for (Member *member : this->users)
        {
            if (member->getUsername() == username)
                res = member;
        }
        return res;
    }


    vector<House *> getAllHouses()
    {
        vector<House *> records;
        for (Member *member : this->users)
        {
            records.push_back(member->getHouseInPossession());
        }
        return records;
    };



    vector<House *> availableHouses(string start, string end)
    {

        vector<House *> allHouses = this->getAllHouses();
        vector<House *> availableRecords;

        for (House *house : allHouses)
            if (house->available(start, end))
            {
                availableRecords.push_back(house);
            }
        return availableRecords;
    }



    vector<House *> availableHousesForMember(Member *member, string start, string end, string city)
    {
        vector<House *> allHouses = this->availableHouses(start, end);
        vector<House *> availableRecords;
        for (House *house : allHouses)
            if (house->getLocation() == city && house->getMinReqOccupierRating() <= member->getOccupierRatingScore())
            {
                long totalCost = house->countDays(start, end) * house->getConsumingPoints();
                if (member->getCreditPoints() >= totalCost)
                    availableRecords.push_back(house);
            }
        return availableRecords;
    }



    void sendRequest(string username, vector<House *> availableHouses, int index, string start, string end)
    {
        if (index >= availableHouses.size())
        {
            cout << "Invalid choice !" << endl;
            return;
        }
        House *house = availableHouses[index];

        Member *member = this->getMemberByUsername(username);

        Request *request = new Request();
        request->setRequestUsername(username);
        request->setStart(start);
        request->setEnd(end);

        vector<Request *> requests = house->getRequests();
        requests.push_back(request);
        house->setRequests(requests);
        cout << "Send request !" << endl;
    }



    void showRequests(Member *member)
    {
        vector<Request *> requests = member->getHouseInPossession()->getRequests();
        if (requests.size() == 0)
        {
            cout << "You don't have any request !" << endl;
            return;
        }
        cout << "All requests for you: " << endl;
        int index = 0;
        for (Request *request : member->getHouseInPossession()->getRequests())
        {
            cout << "Id: " << index << " | User: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
        }
    }



    void delRequests(vector<Request *> &requests, string start, string end)
    {
        Time time = Time();
        tm tmcStart = time.stringToTime(start);
        tm tmcEnd = time.stringToTime(end);

        vector<int> delIndex;
        for (int i = 0; i < requests.size(); i++)
        {
            Request *request = requests[i];
            if (time.compareSmallerTime(time.stringToTime(request->getEnd()), tmcEnd) && time.compareBiggerTime(time.stringToTime(request->getStart()), tmcStart) || time.compareSmallerTime(time.stringToTime(request->getEnd()), tmcEnd) && time.compareBiggerTime(time.stringToTime(request->getEnd()), tmcStart) || time.compareSmallerTime(time.stringToTime(request->getStart()), tmcEnd) && time.compareBiggerTime(time.stringToTime(request->getStart()), tmcStart))
                delIndex.push_back(i);
        }

        int sub = 0;
        for (int index : delIndex)
        {
            int actIndex = index - sub;
            requests.erase(requests.begin() + actIndex);
            sub++;
        }
    }



    void acceptRequest(Member *&member, int index)
    {
        vector<Request *> allRequests = member->getHouseInPossession()->getRequests();

        if (index >= allRequests.size())
        {
            cout << "Invalid choice !" << endl;
            return;
        }

        Request *request = allRequests[index];
        if (member->getHouseInPossession()->available(request->getStart(), request->getEnd()))
        {
            cout << "Successfully accept request from user: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
            allRequests.erase(allRequests.begin() + index);
            member->getHouseInPossession()->setStartDate(request->getStart());
            member->getHouseInPossession()->setEndDate(request->getEnd());
           

            this->getMemberByUsername(request->getRequestUsername())->setCreditPoints(this->getMemberByUsername(request->getRequestUsername())->getCreditPoints() - member->getHouseInPossession()->countDays(request->getStart(), request->getEnd()) * member->getHouseInPossession()->getConsumingPoints());

            this->getMemberByUsername(request->getRequestUsername())->setHouseForRent(member->getHouseInPossession());

            this->getMemberByUsername(request->getRequestUsername())->setOccupyDuration();

            member->getHouseInPossession()->setSpendTimes();

            member->setRenter(this->getMemberByUsername(request->getRequestUsername()));

            member->setCreditPoints(member->getCreditPoints() + member->getHouseInPossession()->countDays(request->getStart(), request->getEnd()) * member->getHouseInPossession()->getConsumingPoints());

            this->delRequests(allRequests, request->getStart(), request->getEnd());

            request->setStatus("Accepted");
            member->getHouseInPossession()->setRequests(allRequests);
        }
        else
            cout << "Your house is not be free on this period ! Please check again" << endl;
    }
    
    /*------------------------------------------------------Registration for non-member-------------------------------------------------*/
    //check if username is used already
    bool checkDupUsername(string username){
        bool isValid = false;
        for(Member *member: this->users){
            if(member->getUsername()==username) {
                isValid = true;
                cout<<"This username has been created before !"<<endl;
                break;
            }
        }
        return isValid;
    }

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
                cout << "\nInvalid phone number. Please try again." << endl;
                cout << "Phone number starts wth 0 and have 10 numbers" << endl;
            }
        }
        return phoneNumber;
    }
    
    //Regis for non-member
    Member *regisNewAccount()
    {
        cout << " ***** Register Account *****\n";
        string data = "";
        Member *newMember = new Member();

        newMember->setIsMember(true);
        data += "\nMember";


        string username;
        while(username=="" ||checkDupUsername(username)) {
            cout << "Enter username: ";
            cin >> username;
        }
        newMember->setUsername(username);
        data += "," + username;


        //Passwword
        cout << "Enter password: ";
        string password;
        cin >> password;
        newMember->setPassword(password);
        data += "," + password;

        //Fullname
        cout << "Enter fullname: ";
        string fullname;
        getline(cin >> ws, fullname); // use getline() to read the fullname including spaces
        newMember->setFullname(fullname);
        data += "," + fullname;

        //Phone
        //Enter phone number (must be 10 digits and start with 0)
        string phone = getValidPhoneNumber(); 
        newMember->setPhone(phone);
        data += "," + phone;


        string enterHouse;
        while(enterHouse!="1" && enterHouse!="2"){
            cout << "Enter house ? Press 1->Yes  2->No : ";
            cin >> enterHouse;
        }
        if (enterHouse == "1")
        {
            cout << "Enter Location: ";
            string location;
            while (location != "Hanoi" && location != "Hue" && location != "Saigon")
            {
                cout << "(Hanoi, Hue or Saigon): ";
                cin >> location;
                data += "," + location;
                if (location != "Hanoi" && location != "Hue" && location != "Saigon")
                {
                    cout << "Invalid location, please enter a valid location." << endl;
                }
            }

            cout << "Enter Description: ";
            string description;
            getline(cin >> ws, description);
            data += "," + description;
            House *house = new House(location, description);
            newMember->setHouseInPossession(house);
        }

        ofstream myfile;
        myfile.open("Data.txt", fstream::app);
        myfile << data;
        cout << "You have successfully registered account";

        return newMember;
    }

    // Manage the period for renting by the owner of the house
    void houseAvailabilityManage(Member *&mem)
    {
        if (mem->getHouseInPossession()->getStartPeriod() != "" && mem->getHouseInPossession()->getEndPeriod() != "")
            cout << "Your house is now available for rented from: " << mem->getHouseInPossession()->getStartPeriod() << " to: " << mem->getHouseInPossession()->getEndPeriod() << endl;
        else
        {
            cout << "Your house is now free on every period" << endl;
        }
        string choice;
        cout << "Do you want to change the period of your house: Y/N" << endl;
        while (choice == "")
        {
            cin >> choice;
            if (choice == "Y" || choice == "y")
            {
                cout << "Date format will be YYYY/MM/DD";
                
                string newStartDate;
                while (true) // validate input
                {
                    cout << "Please enter a start day: " << endl;
                    cin >> newStartDate;
                    if (!isValidDate(newStartDate))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(newStartDate))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }

                string newEndDate;
                while (true) // validate input
                {
                    cout << "Please enter a start day: " << endl;
                    cin >> newEndDate;
                    if (!isValidDate(newEndDate))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(newEndDate))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }

                bool checkSuccess = mem->setAvailablePeriod(newStartDate, newEndDate);
                if (checkSuccess)
                    cout << "Your house is now available for rented from: " << mem->getHouseInPossession()->getStartPeriod() << " to: " << mem->getHouseInPossession()->getEndPeriod() << endl;
                else
                    cout << "START DATE cannot be BIGGER than END DATE" << endl;
            }
        }
    }

    // vector of requests for requester to view the status---------------------------------------------------------------
    vector<Request *> requestsOfOccupier(Member *mem)
    {
        vector<Request *> records;
        vector<House *> allHouses = this->getAllHouses();

        for (House *house : allHouses)
        {
            for (Request *req : house->getRequests())
            {
                if (req->getRequestUsername() == mem->getUsername())
                    records.push_back(req);
            }
        }
        return records;
    }

    // validate user choice
    bool checkChoice(string choice, string start, string end)
    {
        if ((choice.size() > 1) | (choice.size() < 1))
        {
            cout << "\nInvalid Input! Please enter your choice again" << endl
                 << "*********************************************\n";
            return true;
        }
        else if ((start > choice | choice > end) | (!isdigit(choice[0])))
        {
            cout << "\nInvalid Input! Please enter your choice again" << endl
                 << "********************************************\n";
            return true;
        }
        return false;
    }

    /*------------------------------------------------------Login function-------------------------------------------------*/
    Member *Login(vector<Member *> users)
    {
        Member *mem;
        int login_time = 3;
        bool check = false;
        while (!check)
        {
            cout << "\nEnter username: ";
            string username;
            cin >> username;
               
            cout << "Enter password: ";
            string password;
            cin >> password;
            cout << endl;
                
            for (int i = 0; i < users.size(); i++)
            {
                if ((users[i]->getUsername() == username) && (users[i]->getPassword() == password))
                {
                    cout << "Login success!!! Welcome" << users[i]->getFullname();
                    check = true;
                    mem = users[i];
                    break;
                }
            } 

            if (!check)
            {
                login_time--;
                cout << "\nWrong username or wrong password. Please enter again!" << endl;
                
                if (login_time > 0){
                cout << "You have " << login_time << " attempt left";
                }
                
                cout << endl;
            }

            if (login_time == 0)
            {
                cout << "You have exceeded the maximum number of login attempts. Please wait 60s before trying again." << endl;
                this_thread::sleep_for(chrono::seconds(60)); // wait 60 seconds before allowing login again
                login_time = 3; // reset login time after waiting
            }
        }
        return mem;
    }

    // validate if user enter right date format------------------------------------------------------------------------------
    bool isValidDate(string date)
    {
    // Check if the date is in the correct format (YYYY/MM/DD)
    if (date.length() != 10 || date[4] != '/' || date[7] != '/')
    return false;

    // Extract the year, month, and day from the input string
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // Check if the year is within the valid range (1900-9999)
    if (year < 1900 || year > 9999)
        return false;

    // Check if the month is within the valid range (1-12)
    if (month < 1 || month > 12)
        return false;

    // Check if the day is within the valid range for the given month
    if (day < 1 || day > 31)
        return false;
    else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    else if (month == 2 && day > 29)
        return false;
    else if (month == 2 && day == 29)
    {
        // Check if the year is a leap year
        if (year % 4 != 0)
            return false;
        if (year % 100 == 0 && year % 400 != 0)
            return false;
    }

    return true;
    }

    void confirmRole(string role)
    {
        // menu for guest
        if (role == "1")
        {
            cout << "\nThis is your menu:" << endl
                 << "0. Exit" << endl
                 << "1. View houses" << endl
                 << "Enter your choice: " << endl;
        }
        // menu for member
        else if (role == "2")
        {
            cout << "\nThis is your menu:" << endl
                 << "0. Log out" << endl
                 << "1. Manage your house" << endl
                 << "2. Search for available suitable houses" << endl
                 << "3. Rate house" << endl
                 << "4. Rate occupier" << endl
                 << "5. View requests" << endl
                 << "6. View information" << endl
                 << "7. View your house" << endl
                 << "8. Review house" << endl
                 << "Enter your choice: " << endl;
        }
        // Admin function
        else
        {
            cout << "\nThis is your menu:" << endl
                 << "0. Log out" << endl
                 << "1. View all member and houses' information " << endl
                 << "Enter your choice: " << endl;
        }
    }

    // validate choice by user's menu
    void  confirmMenu(string role, string choice)
    {
        if (role == "1")
            checkChoice(choice, "0", "1");
        else if (role == "2")
            checkChoice(choice, "0", "9");
        else
            checkChoice(choice, "0", "1");
    }

    // check function base on user's choice
    void confirmFunction(string role, string choice, Member *&mem, System *&sys)
    {
        // use menu as a guest
        if (role == "1")
        {
            if (choice == "1")
            {
                cout << "Houses Details: " << endl;
                cout << endl;
                int count = 0;
                for (House *house : sys->getAllHouses())
                {
                    count++;
                    cout << count << "." << endl
                         << "Location: " << house->getLocation() << "         | Consuming Points: " << house->getConsumingPoints() << endl
                         << "Descrption: " << house->getDescription() << endl
                         << "Rating : " << house->getHouseRatingScore() << " | Used Times: " << house->getSpendTimes() << endl;
                }
            }
        }
        // use menu as a member
        else if (role == "2")
        {
            // int intChoice = stoi(choice);
            string usname = mem->getRenter() != NULL ? mem->getRenter()->getUsername() : "None";
            switch (choice[0])
            {
                // list/unlist house
                case '1':
                {
                    if(mem->getHouseInPossession()==NULL){
                        cout<<"Currently you register no house ! Register a house (Y/N)"<<endl;
                        string response;
                        cin>>response;
                        if(response=="Y" || response=="y"){
                            cout << "Enter Location: ";
                            string location;
                            cin >> location;

                            cout << "Enter Description: ";
                            string description;
                            cin >> description;
                            House *house = new House(location, description);
                            mem->setHouseInPossession(house);
                            cout<<"Successfully registered a house !"<<endl;
                        }
                    }
                    else { 
                        cout << "\nHouse controller:" << endl
                        << "1. List/Unlist available occupied houses" << endl
                        << "2. Set consuming points for your house" << endl
                        << "3. Set minimum required rating score for your house" << endl;
                        string houseMana;
                        cin >> houseMana;
                        if (houseMana == "1")
                            houseAvailabilityManage(mem);
                        else if (houseMana == "2")
                        {
                            cout << "Enter the consuming points" << endl;
                            double consume;
                            cin >> consume;
                            mem->getHouseInPossession()->setConsumingPoints(consume);
                        }
                        else if (houseMana == "3")
                        {
                            cout << "Enter the minimum required rating score" << endl;
                            double minScore;
                            cin >> minScore;
                            mem->getHouseInPossession()->setMinReqOccupierRating(minScore);
                        }
                    }
                    break;
                }
                // search for suitable house
                case '2':
                {
                    cout << "\nHouses : " << endl
                         << endl;
                    string start, end, city, review;
                    while (true) // validate input
                    {
                        cout << "Enter the start date (YYYY/MM/DD): " << endl;
                        cin >> start;
                        if (!isValidDate(start))
                            cout << "Invalid Input! Renter your date by the given format!" << endl;
                        else if (!isValidDate(start))
                            cout << "Invalid Date!" << endl;
                        else
                            break;
                    }

                    while (true) // validate input
                {
                    cout << "Enter the end date (YYYY/MM/DD): " << endl;
                    cin >> end;
                    if (!isValidDate(end))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(end))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }
                    while (true) // validate input
                    {
                        cout << "Enter the city (Hanoi/ Saigon/ Hue): " << endl;
                        cin >> city;
                        if (city == "Hanoi" && (city != "Saigon") && (city != "Hue"))
                            cout << "Invalid City! Enter the city again!" << endl;
                        else
                            break;
                    }
                    while (true) // validate input
                    {
                        cout << "Do you want to see the reviews also? (Y/N)" << endl;
                        cin >> review;
                        if ((review != "Y") && (review != "y") && (review != "N") && (review != "n"))
                            cout << "Invalid Input! Enter your choice again!" << endl;
                        else
                            break;
                    }

                    int count = 0;
                    vector<House*> avaiHouses = sys->availableHousesForMember(mem, start, end, city);
                    if ((review == "Y") || (review == "y"))
                    {   
                        if(avaiHouses.size()==0)
                            cout<<"Currently no houses for you !"<<endl;
                        else {
                            for (House *house : avaiHouses) // print houses' details with review
                            {
                                if (house == mem->getHouseInPossession())
                                    continue;
                                cout << count << "." << endl
                                     << "Location: " << house->getLocation() << "         | Consuming Points: " << house->getConsumingPoints() << endl
                                     << "Descrption: " << house->getDescription() << endl
                                     << "Rating : " << house->getHouseRatingScore() << " | Used Times: " << house->getSpendTimes() << endl
                                     << "Review : " << endl;
                                for (string cmt : house->getComments())
                                    cout << " " << cmt << endl;
                                cout << endl;
                                count++;
                            }
                        }
                    }
                    else
                    {
                        if(avaiHouses.size()==0)
                            cout<<"Currently no houses for you !"<<endl;
                        else {
                            for (House *house : avaiHouses) // print houses' details without review
                            {
                                if (house == mem->getHouseInPossession())
                                    continue;
                                cout << count << "." << endl
                                    << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                                    << "Rating : " << house->getHouseRatingScore() << "  Used Times: " << house->getSpendTimes() << endl
                                    << endl;
                                count++;
                            }
                        }
                    }

                    if(avaiHouses.size()!=0){
                        while (true) // validate input
                    {
                        cout << "Do you want to make a request? (Y/N)" << endl;
                        cin >> review;
                        if ((review != "Y") && (review != "y") && (review != "N") && (review != "n"))
                            cout << "Invalid Input! Enter your choice again!" << endl;
                        else
                            break;
                    }

                    string num_house;
                    if ((review == "Y") || (review == "y")) // make a request
                    {
                        while (true)
                        {
                            cout << "Pick a house's number which one you would like to make a request?" << endl;
                            cin >> num_house;
                            if ((num_house.size() > 2) && (!isdigit(num_house[0])) && (!isdigit(num_house[1])))
                                cout << "Invalid Input! Enter your choice again!" << endl;
                            else
                                break;
                        }
                        int index = stoi(num_house);
                        sys->sendRequest(mem->getUsername(), sys->availableHouses(start, end), index, start, end);
                    }

                    else if ((review == "N") || (review == "n"))
                        break;
                    }
                    
                    cout << endl;
                    break;
                }

                // rate living house
                case '3':
                    if ((mem->getHouseForRent()) == NULL)
                    {
                        cout << "Invalid house" << endl;
                        break;
                    }
                    cout << "\nPlease rating your living house (from scale -10 to 10): " << endl;
                    float score;
                    while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
                    {
                        cout << "ERROR: a number must be entered " << endl
                            << endl;
                        cin.clear();
                        cin.ignore(123, '\n');
                        cout << "Please rating your living house (from scale -10 to 10): " << endl;
                    }

                    mem->ratingHouse(score, mem->getHouseForRent());
                    cout << "\nThank you for rating!" << endl;
                    break;
                // rate occupiers
                case '4':
                    if ((mem->getRenter()) == NULL)
                    {
                        cout << "Invalid occupier" << endl;
                        break;
                    }
                    cout << "\nPlease rating your occupiers (from scale -10 to 10): " << endl;
                    // float score;
                    while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
                    {
                        cout << "ERROR: a number must be entered " << endl
                            << endl;
                        cin.clear();
                        cin.ignore(123, '\n');
                        cout << "Please rating your occupiers (from scale -10 to 10): " << endl;
                    }

                    mem->ratingOccupier(score, mem->getRenter());
                    cout << "\nThank you for rating!" << endl;
                    break;
                // view requests
                case '5':
                {
                    int reqChoice;
                    cout << "\nRequests Management:" << endl
                        << "0. Exit" << endl
                        << "1. Received requests " << endl
                        << "2. Send requests" << endl;
                    cin >> reqChoice;
                    if (reqChoice == 1)
                    {
                        if (mem->getHouseInPossession()->getRequests().size() == 0)
                            cout << "No requests" << endl;
                        else
                        {
                            cout << "\nRequest: " << endl
                                << endl;
                            int cnt = 0;
                            string start;
                            string end;
                            for (Request *req : mem->getHouseInPossession()->getRequests())
                                cout << cnt++ << " | "
                                    << "Requested Username: " << req->getRequestUsername() << endl
                                    << "Start Date - End Date: " << req->getStart() << "-" << req->getEnd() << endl;
                            int numReq;
                            cout << "Enter the number of request: " << endl;
                            cin >> numReq;
                            sys->acceptRequest(mem, numReq);
                        }
                    }
                    else if (reqChoice == 2)
                    {
                        vector<Request *> allReqs = this->requestsOfOccupier(mem);
                        if (allReqs.size() == 0)
                        {
                            cout << "Currently no requests" << endl;
                        }
                        else
                        {
                            int cnt = 0;
                            for (Request *req : allReqs)
                            {
                                cout << cnt++ << " | "
                                    << "Start Date - End Date: " << req->getStart() << "-" << req->getEnd() << endl
                                    << "Status: " << req->getStatus() << endl;
                            }
                        }
                    }
                    break;
                }
                    // view info
                case '6':
                {
                    cout << "\nPersonal Info: " << endl
                        << endl
                        << "Fullname: " << mem->getFullname() << endl
                        << "Phone: " << mem->getPhone() << endl
                        << "Credit Points : " << mem->getCreditPoints() << endl
                        << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                        << "Occupied Times : " << mem->getOccupyDuration() << endl
                        << "Username : " << mem->getUsername() << endl
                        << "Present Renter : " << usname << endl
                        << endl;
                    break;
                }
                    // view House info
                case '7':
                {            
                    if(mem->getHouseInPossession()!=NULL){
                        cout << "\nHouse Info: " << endl
                        << endl
                        << "Location: " << mem->getHouseInPossession()->getLocation() << endl
                        << "Consuming Points: " << mem->getHouseInPossession()->getConsumingPoints() << endl
                        << "Rating Score: " << mem->getHouseInPossession()->getHouseRatingScore() << endl
                        << "Occupying Start Date : " << mem->getHouseInPossession()->getStartDate() << endl
                        << "Occupying End Date : " << mem->getHouseInPossession()->getEndDate() << endl
                        << "Available for renting Start : " << mem->getHouseInPossession()->getStartPeriod() << endl
                        << "Available for renting End : " << mem->getHouseInPossession()->getEndPeriod() << endl
                        << endl;
                    }
                    else cout<<"Currently you register no house !"<<endl;
                    break;
                }
                case '8':
                {
                    if (mem->getHouseForRent() != NULL)
                    {
                        vector<string> cmts = mem->getHouseForRent()->getComments();
                        cout << "Enter your review: " << endl;
                        string cmt;
                        cin >> cmt;
                        cmts.push_back(cmt);
                        mem->getHouseForRent()->setComments(cmts);
                    }
                    else
                        cout << "You haven't chosen any house for rent" << endl;
                    break;
                }
            }
        }
        // use menu as an admin
        else if (role == "3")
        {
            //int intChoice = stoi(choice);
            string usname = mem->getRenter() != NULL ? mem->getRenter()->getUsername() : "None";
            switch (choice[0])
            {
                case '1':
                {
                    int count = 0;
                    cout << "\nPeople Info: " << endl
                        << endl;

                    for (Member *mem : sys->users)
                    {
                        string usname = mem->getRenter() != NULL ? mem->getRenter()->getUsername() : "None";
                        count++;
                        cout << count << "." << endl
                            << "Fullname: " << mem->getFullname() << endl
                            << "Phone: " << mem->getPhone() << endl
                            << "Credit Points : " << mem->getCreditPoints() << endl
                            << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                            << "Occupied Times : " << mem->getOccupyDuration() << endl
                            << "Username : " << mem->getUsername() << endl
                            << "Present Partner : " << usname << endl
                            << endl;
                    }

                    cout << "Houses Details: " << endl;
                    cout << endl;
                    count = 0;
                    for (House *house : sys->getAllHouses())
                    {
                        count++;
                        cout << count << "." << endl
                            << "Location: " << house->getLocation() << "         | Consuming Points: " << house->getConsumingPoints() << endl
                            << "Rating : " << house->getHouseRatingScore() << " | Used Times: " << house->getSpendTimes() << endl
                            << endl;
                    }
                    break;
                }
            }
        }
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

    //Skip Read whiteSpace
    void skipWhitespace(ifstream& file) {
        char c = file.peek();  // Look at the next character in the stream
        while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            file.get();  // Read the character
            c = file.peek();  // Look at the next character
        }
    }

    // Read all datas from file
    void readFile() {
        string output;
        ifstream myfile("Data.txt");
        getline(myfile, output); // skip first line
        vector<string> datas;
        while (getline(myfile, output)) {
            // isAdmin, username, passsword, fullname, phone, location, description
            datas.push_back(output);
        }
        // create user and house to add to system
        for (string data : datas) {
            vector<string> splitData = splitString(data);

            Member *m = new Member();
            bool isMember = splitData[0] == "true" ? true : false;
            m->setIsMember(isMember);
            m->setUsername(splitData[1]);
            m->setPassword(splitData[2]);
            m->setFullname(splitData[3]);
            m->setPhone(splitData[4]);

            House *h = new House();
            h->setLocation(splitData[5]);
            h->setDescription(splitData[6]);
            m->setHouseInPossession(h);

            this->users.push_back(m);
        }

        myfile.close();
    }   
};


