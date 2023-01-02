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

    // Get member by username
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

    // Get all the houses on the system
    vector<House *> getAllHouses()
    {
        vector<House *> records;
        for (Member *member : this->users)
        {
            records.push_back(member->getHouseForOwn());
        }
        return records;
    };

    // Show list of houses to be used in a the period
    vector<House *> availableHouses(string start, string end)
    {

        vector<House *> allHouses = this->getAllHouses();
        vector<House *> availableRecords;

        for (House *house : allHouses)
            if (house->isFree(start, end))
            {
                availableRecords.push_back(house);
            }
        return availableRecords;
    }

    // Show list of houses for member
    vector<House *> availableHousesForMember(Member *member, string start, string end, string city)
    {
        vector<House *> allHouses = this->availableHouses(start, end);
        vector<House *> availableRecords;
        for (House *house : allHouses)
            if (house->getLocation() == city && house->getRequiredMinOccupierRating() <= member->getOccupierRatingScore())
            {
                long totalCost = house->countDays(start, end) * house->getConsumingPoints();
                if (member->getCreditPoints() >= totalCost)
                    availableRecords.push_back(house);
            }
        return availableRecords;
    }

    // Send request to owner of the house
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

    // Show all requests for the owner of his house
    void showRequests(Member *member)
    {
        vector<Request *> requests = member->getHouseForOwn()->getRequests();
        if (requests.size() == 0)
        {
            cout << "You don't have any request !" << endl;
            return;
        }
        cout << "All requests for you: " << endl;
        int index = 0;
        for (Request *request : member->getHouseForOwn()->getRequests())
        {
            cout << "Id: " << index << " | User: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
        }
    }

    // Delete appropriate request
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

    // Owner check the requests for accept
    void acceptRequest(Member *&member, int index)
    {
        vector<Request *> allRequests = member->getHouseForOwn()->getRequests();

        if (index >= allRequests.size())
        {
            cout << "Invalid choice !" << endl;
            return;
        }

        Request *request = allRequests[index];
        if (member->getHouseForOwn()->isFree(request->getStart(), request->getEnd()))
        {
            cout << "Successfully accept request from user: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
            allRequests.erase(allRequests.begin() + index);
            member->getHouseForOwn()->setStartDate(request->getStart());
            member->getHouseForOwn()->setEndDate(request->getEnd());
            // cout << member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) << endl;
            // Update credits amount of renter
            this->getMemberByUsername(request->getRequestUsername())->setCreditPoints(this->getMemberByUsername(request->getRequestUsername())->getCreditPoints() - member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) * member->getHouseForOwn()->getConsumingPoints());
            // Update house for live of renter
            this->getMemberByUsername(request->getRequestUsername())->setHouseForLive(member->getHouseForOwn());
            // Update occupy times of renter
            this->getMemberByUsername(request->getRequestUsername())->setOccupyTimes();
            // Update used times of house
            member->getHouseForOwn()->setUsedTimes();
            // Update partner for owner
            member->setPartner(this->getMemberByUsername(request->getRequestUsername()));
            // Update credit points of the owner
            member->setCreditPoints(member->getCreditPoints() + member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) * member->getHouseForOwn()->getConsumingPoints());
            // this->showRequests(member);
            this->delRequests(allRequests, request->getStart(), request->getEnd());
            // Change status of request
            request->setStatus("Accepted");
            member->getHouseForOwn()->setRequests(allRequests);
        }
        else
            cout << "Your house is not be free on this period ! Please check again" << endl;
    }

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
        string AdminOrUser;
        while(AdminOrUser=="" || !isdigit(AdminOrUser[0]) || AdminOrUser.size()>1){
            cout << "Register as 1->Admin  2->Member: ";
            cin >> AdminOrUser;
        }
        Member *newMember = new Member();

        if (AdminOrUser == "1")
        {
            newMember->setIsAdmin(true);
            data += "\ntrue";
        }
        else
        {
            newMember->setIsAdmin(false);
            data += "\nfalse";
        }

        
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
        cout << "You have successfully registered account";

        return newMember;
    }
    
    // Manage the period for renting by the owner of the house
    void houseAvailabilityManage(Member *&mem)
    {
        if (mem->getHouseForOwn()->getAvailablePeriodStart() != "" && mem->getHouseForOwn()->getAvailablePeriodEnd() != "")
            cout << "House is now available for rented started: " << mem->getHouseForOwn()->getAvailablePeriodStart() << " to: " << mem->getHouseForOwn()->getAvailablePeriodEnd() << endl;
        else
        {
            cout << "House is now free on every period" << endl;
        }
        string choice;
        cout << "Do you want to change the period of your house: Y/N" << endl;
        while (choice == "")
        {
            cin >> choice;
            if (choice == "Y" || choice == "y")
            {
                string newStartDate;
                cout << "Please enter a start day: " << endl;
                cin >> newStartDate;

                string newEndDate;
                cout << "Please enter an end day: " << endl;
                cin >> newEndDate;

                bool checkSuccess = mem->setAvailablePeriod(newStartDate, newEndDate);
                if (checkSuccess)
                    cout << "Your house is now available for rented from: " << mem->getHouseForOwn()->getAvailablePeriodStart() << " to: " << mem->getHouseForOwn()->getAvailablePeriodEnd() << endl;
                else
                    cout << "START DATE cannot be BIGGER than END DATE" << endl;
            }
        }
    }

    // vector of requests for requester to view the status
    vector < Request*> requestsOfOccupier(Member *mem)
    {
        vector < Request*> records;
        vector < House*> allHouses = this->getAllHouses();

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

    // Login Function
    bool login(string username, string password) {
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
            if (username == username_from_file && password == password_from_file) {
                return true;
            }
        }   

        return false;
    }

    // validate if user enter right date format
    bool checkDate(string str)
    {
        if (str.size() == 10)
        {
            for (int i = 0; i < str.size(); i++)
            {
                if ((!isdigit(str[i])) & ((i != 4) & (i != 7)))
                    return false;
                if ((str[7] != '/') | (str[4] != '/'))
                    return false;
            }
        }
        return true;
    }

    // Check if the year is leap year
    bool isLeap(int year)
    {
        // Return true if year
        // is a multiple of 4 and
        // not multiple of 100.
        // OR year is multiple of 400.
        return (((year % 4 == 0) &&
                 (year % 100 != 0)) ||
                (year % 400 == 0));
    }

    // Returns true if given
    // year is valid or not.
    bool isValidDate(string y, string m, string d)
    {
        int day = stoi(d);
        int month = stoi(m);
        int year = stoi(y);

        // If year, month and day
        // are not in given range
        if (year > MAX_VALID_YR ||
            year < MIN_VALID_YR)
            return false;
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > 31)
            return false;

        // Handle February month
        // with leap year
        if (month == 2)
        {
            if (isLeap(year))
                return (day <= 29);
            else
                return (day <= 28);
        }

        // Months of April, June,
        // Sept and Nov must have
        // number of days less than
        // or equal to 30.
        if (month == 4 || month == 6 ||
            month == 9 || month == 11)
            return (day <= 30);

        return true;
    }

    // Check role
    void checkRole(string role)
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
        // menu for admin
        else
        {
            cout << "\nThis is your menu:" << endl
                 << "0. Log out" << endl
                 << "1. Manage your house " << endl
                 << "2. Search for available suitable houses" << endl
                 << "3. Rate house" << endl
                 << "4. Rate occupier" << endl
                 << "5. View requests" << endl
                 << "6. View information" << endl
                 << "7. View others information" << endl
                 << "8. Review house" << endl
                 << "Enter your choice: " << endl;
        }
    }

    // validate choice by user's menu
    void checkMenu(string role, string choice)
    {
        if (role == "1")
            checkChoice(choice, "0", "1");
        else if (role == "2")
            checkChoice(choice, "0", "9");
        else
            checkChoice(choice, "0", "10");
    }

    // check function base on user's choice
    void checkFunction(string role, string choice, Member *&mem, System *&sys)
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
                         << "Rating : " << house->getHouseRatingScore() << " | Used Times: " << house->getUsedTimes() << endl;
                }
            }
        }
        // use menu as a member
        else if (role == "2")
        {
            // int intChoice = stoi(choice);
            string usname = mem->getPartner() != NULL ? mem->getPartner()->getUsername() : "None";
            switch (choice[0])
            {
            // list/unlist house
            case '1':
            {
                if(mem->getHouseForOwn()==NULL){
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
                        mem->setHouseForOwn(house);
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
                        mem->getHouseForOwn()->setConsumingPoints(consume);
                    }
                    else if (houseMana == "3")
                    {
                        cout << "Enter the minimum required rating score" << endl;
                        double minScore;
                        cin >> minScore;
                        mem->getHouseForOwn()->setRequiredMinOccupierRating(minScore);
                    }
                }
                break;
            }
            // rate living house
            case '3':
                if ((mem->getHouseForLive()) == NULL)
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

                mem->ratingHouse(score, mem->getHouseForLive());
                cout << "\nThank you for rating!" << endl;
                break;
            // rate occupiers
            case '4':
                if ((mem->getPartner()) == NULL)
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

                mem->ratingOccupier(score, mem->getPartner());
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
                    if (mem->getHouseForOwn()->getRequests().size() == 0)
                        cout << "No requests" << endl;
                    else
                    {
                        cout << "\nRequest: " << endl
                             << endl;
                        int cnt = 0;
                        string start;
                        string end;
                        for (Request *req : mem->getHouseForOwn()->getRequests())
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
                cout << "\nPersonal Info: " << endl
                     << endl
                     << "Fullname: " << mem->getFullname() << endl
                     << "Phone: " << mem->getPhone() << endl
                     << "Credit Points : " << mem->getCreditPoints() << endl
                     << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                     << "Occupied Times : " << mem->getOccupyTimes() << endl
                     << "Username : " << mem->getUsername() << endl
                     << "Present Partner : " << usname << endl
                     << endl;
                break;
            // view House info
            case '7':
                if(mem->getHouseForOwn()!=NULL){
                    cout << "\nHouse Info: " << endl
                     << endl
                     << "Location: " << mem->getHouseForOwn()->getLocation() << endl
                     << "Consuming Points: " << mem->getHouseForOwn()->getConsumingPoints() << endl
                     << "Rating Score: " << mem->getHouseForOwn()->getHouseRatingScore() << endl
                     << "Occupying Start Date : " << mem->getHouseForOwn()->getStartDate() << endl
                     << "Occupying End Date : " << mem->getHouseForOwn()->getEndDate() << endl
                     << "Available for renting Start : " << mem->getHouseForOwn()->getAvailablePeriodStart() << endl
                     << "Available for renting End : " << mem->getHouseForOwn()->getAvailablePeriodStart() << endl
                     << endl;
                }
                else cout<<"Currently you register no house !"<<endl;
                break;
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
                    if (!checkDate(start))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(start.substr(0, 4), start.substr(5, 2), start.substr(8, 2)))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }

                while (true) // validate input
                {
                    cout << "Enter the end date (YYYY/MM/DD): " << endl;
                    cin >> end;
                    if (!checkDate(end))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(end.substr(0, 4), end.substr(5, 2), end.substr(8, 2)))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }
                while (true) // validate input
                {
                    cout << "Enter the city (Hanoi/ Saigon/ Danang): " << endl;
                    cin >> city;
                    if (city == "Hanoi" && (city != "Saigon") && (city != "Danang"))
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
                            if (house == mem->getHouseForOwn())
                                continue;
                            cout << count << "." << endl
                                << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                                << "Rating : " << house->getHouseRatingScore() << "  Used Times: " << house->getUsedTimes() << endl
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
                            if (house == mem->getHouseForOwn())
                                continue;
                            cout << count << "." << endl
                                << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                                << "Rating : " << house->getHouseRatingScore() << "  Used Times: " << house->getUsedTimes() << endl
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

            case '8':
                if (mem->getHouseForLive() != NULL)
                {
                    vector<string> cmts = mem->getHouseForLive()->getComments();
                    cout << "Enter your review: " << endl;
                    string cmt;
                    cin >> cmt;
                    cmts.push_back(cmt);
                    mem->getHouseForLive()->setComments(cmts);
                }
                else
                    cout << "You haven't chosen any house for rent" << endl;
                break;
            }
        }
        // use menu as an admin
        else if (role == "3")
        {
            // int intChoice = stoi(choice);
            string usname = mem->getPartner() != NULL ? mem->getPartner()->getUsername() : "None";
            switch (choice[0])
            {
            // House management
            case '1':
                if(mem->getHouseForOwn()==NULL){
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
                        mem->setHouseForOwn(house);
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
                        mem->getHouseForOwn()->setConsumingPoints(consume);
                    }
                    else if (houseMana == "3")
                    {
                        cout << "Enter the minimum required rating score" << endl;
                        double minScore;
                        cin >> minScore;
                        mem->getHouseForOwn()->setRequiredMinOccupierRating(minScore);
                    }
                }
                break;
            // rate living house
            case '3':
                if ((mem->getHouseForLive()) == NULL)
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

                mem->ratingHouse(score, mem->getHouseForLive());
                cout << "\nThank you for rating!" << endl;
                break;
            // rate occupiers
            case '4':
                if ((mem->getPartner()) == NULL)
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

                mem->ratingOccupier(score, mem->getPartner());
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
                    if (mem->getHouseForOwn()->getRequests().size() == 0)
                        cout << "No requests" << endl;
                    else
                    {
                        cout << "\nRequest: " << endl
                             << endl;
                        int cnt = 0;
                        string start;
                        string end;
                        for (Request *req : mem->getHouseForOwn()->getRequests())
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
                cout << "\nPersonal Info: " << endl
                     << endl
                     << "Fullname: " << mem->getFullname() << endl
                     << "Phone: " << mem->getPhone() << endl
                     << "Credit Points : " << mem->getCreditPoints() << endl
                     << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                     << "Occupied Times : " << mem->getOccupyTimes() << endl
                     << "Username : " << mem->getUsername() << endl
                     << "Present Partner : " << usname << endl
                     << endl;

                if(mem->getHouseForOwn()!=NULL){
                    cout << "\nHouse Info: " << endl
                     << endl
                     << "Location: " << mem->getHouseForOwn()->getLocation() << endl
                     << "Consuming Points: " << mem->getHouseForOwn()->getConsumingPoints() << endl
                     << "Rating Score: " << mem->getHouseForOwn()->getHouseRatingScore() << endl
                     << "Occupying Start Date : " << mem->getHouseForOwn()->getStartDate() << endl
                     << "Occupying End Date : " << mem->getHouseForOwn()->getEndDate() << endl
                     << "Available for renting Start : " << mem->getHouseForOwn()->getAvailablePeriodStart() << endl
                     << "Available for renting End : " << mem->getHouseForOwn()->getAvailablePeriodStart() << endl
                     << endl;
                }
                else cout<<"Currently you register no house !"<<endl;
                break;
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
                    if (!checkDate(start))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(start.substr(0, 4), start.substr(5, 2), start.substr(8, 2)))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }

                while (true) // validate input
                {
                    cout << "Enter the end date (YYYY/MM/DD): " << endl;
                    cin >> end;
                    if (!checkDate(end))
                        cout << "Invalid Input! Renter your date by the given format!" << endl;
                    else if (!isValidDate(end.substr(0, 4), end.substr(5, 2), end.substr(8, 2)))
                        cout << "Invalid Date!" << endl;
                    else
                        break;
                }
                while (true) // validate input
                {
                    cout << "Enter the city (Hanoi/ Saigon/ Danang): " << endl;
                    cin >> city;
                    if ((city != "Hanoi") && (city != "Saigon") && (city != "Danang"))
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

                vector<House*> avaiHouses = sys->availableHousesForMember(mem, start, end, city);
                int count = 0;
                if ((review == "Y") | (review == "y"))
                {   

                    if(avaiHouses.size()==0)
                        cout<<"Currently no houses for you !"<<endl;
                    else{
                        for (House *house : avaiHouses) // print houses' details with review
                        {
                            count++;
                            cout << count << "." << endl;
                            cout << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                                << "Rating : " << house->getHouseRatingScore() << "  Used Times: " << house->getUsedTimes() << endl
                                << "Review : " << endl;
                            for (string cmt : house->getComments())
                                cout << "" << cmt << endl;
                            cout << endl;
                        }
                    }
                }
                else
                {   
                    if(avaiHouses.size()==0)
                        cout<<"Currently no houses for you !"<<endl;
                    else {
                        for (House *house : sys->availableHousesForMember(mem, start, end, city)) // print houses' details without review
                        {
                            count++;
                            cout << count << "." << endl;
                            cout << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                                << "Rating : " << house->getHouseRatingScore() << "  Used Times: " << house->getUsedTimes() << endl
                                << end;
                        }
                        cout << endl;
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
                        if ((num_house.size() > 2) | (!isdigit(num_house[0])) | (!isdigit(num_house[1])))
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
            // view everyone and houses info
            case '7':
            {
                int count = 0;
                cout << "\nPeople Info: " << endl
                     << endl;

                for (Member *mem : sys->users)
                {
                    string usname = mem->getPartner() != NULL ? mem->getPartner()->getUsername() : "None";
                    count++;
                    cout << count << "." << endl
                         << "Fullname: " << mem->getFullname() << endl
                         << "Phone: " << mem->getPhone() << endl
                         << "Credit Points : " << mem->getCreditPoints() << endl
                         << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                         << "Occupied Times : " << mem->getOccupyTimes() << endl
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
                         << "Rating : " << house->getHouseRatingScore() << " | Used Times: " << house->getUsedTimes() << endl
                         << endl;
                }
                break;
            }
            case '8':
                if (mem->getHouseForLive() != NULL)
                {
                    vector<string> cmts = mem->getHouseForLive()->getComments();
                    cout << "Enter your review: " << endl;
                    string cmt;
                    cin >> cmt;
                    cmts.push_back(cmt);
                    mem->getHouseForLive()->setComments(cmts);
                }
                else
                    cout << "You haven't chosen any house for rent" << endl;
                break;
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

