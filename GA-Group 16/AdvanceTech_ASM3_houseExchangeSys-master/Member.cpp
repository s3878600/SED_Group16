#include <iostream>
#include <stdio.h>
#include <cstring>
#include "House.cpp"

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
	House *houseForLive;
	House *houseForOwn;
	long occupyTimes;
	bool isAdmin;
	Member *partner;

public:
	// Constructor
	Member()
	{
		this->occupyTimes = 0;
		this->occupierRatingScore = 0;
		this->partner = NULL;
		this->creditPoints = 500;
		this->houseForLive == NULL;
	}

	Member(string username, string password, string fullname, string phone)
	{
		this->username = username;
		this->password = password;
		this->fullname = fullname;
		this->phone = phone;
		this->partner = NULL;
		this->occupyTimes = 0;
		this->creditPoints = 500;
		this->houseForLive == NULL;
		this->occupierRatingScore = 0;
	}

	// Getter - Setter
	string getUsername()
	{
		return this->username;
	}

	void setUsername(string username)
	{
		this->username = username;
	}

	string getFullname()
	{
		return this->fullname;
	}

	void setFullname(string fullname)
	{
		this->fullname = fullname;
	}

	string getPhone()
	{
		return this->phone;
	}

	void setPhone(string phone)
	{
		this->phone = phone;
	}

	string getPassword()
	{
		return this->password;
	}

	void setPassword(string password)
	{
		this->password = password;
	}

	long getCreditPoints()
	{
		return this->creditPoints;
	}

	void setCreditPoints(long creditPoints)
	{
		this->creditPoints = creditPoints;
	}

	double getOccupierRatingScore()
	{
		return this->occupierRatingScore;
	}

	void setOccupierRatingScore(double occupierRatingScore)
	{
		this->occupierRatingScore = occupierRatingScore;
	}

	House *getHouseForLive()
	{
		return this->houseForLive;
	}

	void setHouseForLive(House *houseForLive)
	{
		this->houseForLive = houseForLive;
	}

	House *getHouseForOwn()
	{
		return this->houseForOwn;
	}

	void setHouseForOwn(House *houseForOwn)
	{
		this->houseForOwn = houseForOwn;
	}

	long getOccupyTimes()
	{
		return this->occupyTimes;
	}

	void setOccupyTimes()
	{
		this->occupyTimes++;
	}

	bool getIsAdmin()
	{
		return this->isAdmin;
	}

	void setIsAdmin(bool isAdmin)
	{
		this->isAdmin = isAdmin;
	}

	void setPartner(Member *partner)
	{
		this->partner = partner;
	}

	Member *getPartner()
	{
		return this->partner;
	}

	// Rate the House while living
	void ratingHouse(double rating, House *house)
	{
		double newRating = (house->getHouseRatingScore() + rating) / house->getUsedTimes();
		house->setHouseRatingScore(newRating);
	}

	// Rate the Occupier while renting
	void ratingOccupier(double rating, Member *member)
	{
		double newRating = (member->getOccupierRatingScore() + rating) / member->getOccupyTimes();
		member->setOccupierRatingScore(newRating);
	}

	// Set the period which the owner wants to rent his house
	bool setAvailablePeriod(string start, string end)
	{
		Time time = Time();
		tm tmStart = time.stringToTime(start);
		tm tmEnd = time.stringToTime(end);
		if (time.compareBiggerTime(tmStart, tmEnd))
		{
			return false;
		}
		this->getHouseForOwn()->setAvailablePeriodStart(start);
		this->getHouseForOwn()->setAvailablePeriodEnd(end);
		return true;
	}

	friend class System;
};
