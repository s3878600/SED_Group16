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
	: username(username), password(password), fullname(fullname), phone(phone), partner(nullptr), occupyTimes(0), creditPoints(500), occupierRatingScore(0){}

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

	House *getHouseForLive()
	{
		return houseForLive;
	}

	void setHouseForLive(House *houseForLive)
	{
		houseForLive = houseForLive;
	}

	House *getHouseForOwn()
	{
		return houseForOwn;
	}

	void setHouseForOwn(House *houseForOwn)
	{
		houseForOwn = houseForOwn;
	}

	long getOccupyTimes()
	{
		return occupyTimes;
	}

	void setOccupyTimes()
	{
		occupyTimes++;
	}

	bool getIsAdmin()
	{
		return isAdmin;
	}

	void setIsAdmin(bool isAdmin)
	{
		isAdmin = isAdmin;
	}

	void setPartner(Member *partner)
	{
		partner = partner;
	}

	Member *getPartner()
	{
		return partner;
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
