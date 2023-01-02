#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

// Member class
class Member
{
private:
	string username;
	string fullname;
	string phone;
	string password;
	long creditPoints;
	double occupierRatingScore;
	long occupyTimes;
	Member *partner;

public:
	// Constructor

    Member(): partner(nullptr), occupyTimes(0), creditPoints(500), occupierRatingScore(0){}
	
    Member(string username, string password, string fullname, string phone)
	: username(username), password(password), fullname(fullname), phone(phone), partner(nullptr), occupyTimes(0), creditPoints(500), occupierRatingScore(0){}

    //Function:
    
    // Login 

	// Getter - Setter
	string getUsername()
	{
		return username;
	}

	void setUsername(string username)
	{
		username = username;
	}

	string getFullname()
	{
		return fullname;
	}

	void setFullname(string fullname)
	{
		fullname = fullname;
	}

	string getPhone()
	{
		return phone;
	}

	void setPhone(string phone)
	{
		phone = phone;
	}

	string getPassword()
	{
		return password;
	}

	void setPassword(string password)
	{
		password = password;
	}

	long getCreditPoints()
	{
		return creditPoints;
	}

	void setCreditPoints(long creditPoints)
	{
		creditPoints = creditPoints;
	}

	double getOccupierRatingScore()
	{
		return occupierRatingScore;
	}

	void setOccupierRatingScore(double occupierRatingScore)
	{
		occupierRatingScore = occupierRatingScore;
	}

	long getOccupyTimes()
	{
		return occupyTimes;
	}

	void setOccupyTimes()
	{
		occupyTimes++;
	}

	void setPartner(Member *partner)
	{
		partner = partner;
	}

	Member *getPartner()
	{
		return partner;
	}
};