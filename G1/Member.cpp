#include <iostream>
#include <stdio.h>
#include <cstring>
#include "House.cpp"

using namespace std;


class Member
{
private:
	string username;
	string fullname;
	string phone;
	string password;
	long creditPoints;
	double occupierRatingScore;
	House *houseInPossession;
	House *houseForRent;
	long occupyDuration;
	bool isMember;
	Member *renter;

public:
	Member()
	{
		this->occupyDuration = 0;
		this->occupierRatingScore = 0;
		this->renter = NULL;
		this->creditPoints = 500;
		this->houseInPossession == NULL;
	}

	Member(string username, string password, string fullname, string phone)
	{
		this->username = username;
		this->password = password;
		this->fullname = fullname;
		this->phone = phone;
		this->renter = NULL;
		this->occupyDuration = 0;
		this->creditPoints = 500;
		this->houseInPossession == NULL;
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

	House *getHouseForRent()
	{
		return this->houseForRent;
	}

	void setHouseForRent(House *houseForRent)
	{
		this->houseForRent = houseForRent;
	}

	House *getHouseInPossession()
	{
		return this->houseInPossession;
	}

	void setHouseInPossession(House *houseInPossession)
	{
		this->houseInPossession = houseInPossession;
	}

	long getOccupyDuration()
	{
		return this->occupyDuration;
	}

	void setOccupyDuration()
	{
		this->occupyDuration++;
	}

	bool getIsMember()
	{
		return this->isMember;
	}

	void setIsMember(bool isMember)
	{
		this->isMember = isMember;
	}

	void setRenter(Member *renter)
	{
		this->renter = renter;
	}

	Member *getRenter()
	{
		return this->renter;
	}


	void ratingHouse(double rating, House *house)
	{
		double newRating = (house->getHouseRatingScore() + rating) / house->getSpendTimes();
		house->setHouseRatingScore(newRating);
	}


	void ratingOccupier(double rating, Member *member)
	{
		double newRating = (member->getOccupierRatingScore() + rating) / member->getOccupyDuration();
		member->setOccupierRatingScore(newRating);
	}


	bool setAvailablePeriod(string start, string end)
	{
		Time time = Time();
		tm tmStart = time.stringToTime(start);
		tm tmEnd = time.stringToTime(end);
		if (time.compareBiggerTime(tmStart, tmEnd))
		{
			return false;
		}
		this->getHouseInPossession()->setStartPeriod(start);
		this->getHouseInPossession()->setEndPeriod(end);
		return true;
	}

	friend class System;
};
