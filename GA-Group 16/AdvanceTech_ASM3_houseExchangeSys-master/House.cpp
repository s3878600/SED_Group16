#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include "Request.cpp"
#include "Time.cpp"

using namespace std;

class House
{
private:
	string location;
	string descripton;
	double houseRatingScrore;
	string startDate;
	string endDate;
	string availablePeriodStart;
	string availablePeriodEnd;
	double consumingPoints; // per day
	double requiredMinOccupierRating;
	long usedTimes;
	long usedDays;
	vector<string> comments;
	vector<Request *> requests;

public:
	House()
	{
		this->houseRatingScrore = 0;
		this->consumingPoints = 0;
		this->usedTimes = 0;
		this->requiredMinOccupierRating = 0;
	};

	House(string location, string descripton)
	{
		this->location = location;
		this->descripton = descripton;
		this->usedTimes = 0;
		this->requiredMinOccupierRating = 0;
		this->houseRatingScrore = 0;
	}

	string getLocation()
	{
		return this->location;
	}

	void setLocation(string location)
	{
		this->location = location;
	}

	string getDescription()
	{
		return this->descripton;
	}

	void setDescription(string descripton)
	{
		this->descripton = descripton;
	}

	double getHouseRatingScore()
	{
		return this->houseRatingScrore;
	}

	void setHouseRatingScore(double houseRatingScore)
	{
		this->houseRatingScrore = houseRatingScore;
	}

	string getStartDate()
	{
		return this->startDate;
	}

	void setStartDate(string startDate)
	{
		this->startDate = startDate;
	}

	string getEndDate()
	{
		return this->endDate;
	}

	void setEndDate(string endDate)
	{
		this->endDate = endDate;
	}

	void setAvailablePeriodStart(string availablePeriodStart)
	{
		this->availablePeriodStart = availablePeriodStart;
	}

	string getAvailablePeriodEnd()
	{
		return this->availablePeriodEnd;
	}

	void setAvailablePeriodEnd(string availablePeriodEnd)
	{
		this->availablePeriodEnd = availablePeriodEnd;
	}

	string getAvailablePeriodStart()
	{
		return this->availablePeriodStart;
	}

	double getConsumingPoints()
	{
		return this->consumingPoints;
	}

	// void newConsumingPoints(){
	// 	this->setConsumingPoints(this->getUsedTimes()/this->getUsedDays());
	// }

	void setConsumingPoints(double consumingPoints)
	{
		this->consumingPoints = consumingPoints;
	}

	long getUsedTimes()
	{
		return this->usedTimes;
	}

	void setUsedTimes()
	{
		this->usedTimes++;
	}

	double getRequiredMinOccupierRating()
	{
		return this->requiredMinOccupierRating;
	}

	void setRequiredMinOccupierRating(double requiredMinOccupier)
	{
		this->requiredMinOccupierRating = requiredMinOccupier;
	}

	void setUsedDays(long usedDays)
	{
		this->usedDays = usedDays;
	}

	long getUsedDays()
	{
		return this->usedDays;
	}

	void setComments(vector<string> comments)
	{
		this->comments = comments;
	}

	vector<string> getComments()
	{
		return this->comments;
	}

	void setRequests(vector<Request *> requests)
	{
		this->requests = requests;
	}

	vector<Request *> getRequests()
	{
		return this->requests;
	}

	// Check if the house if free between start and end
	bool isFree(string start, string end)
	{
		Time time = Time();
		tm tmStart = time.stringToTime(start);
		tm tmEnd = time.stringToTime(end);

		if (time.compareBiggerTime(tmStart, tmEnd))
			return false;

		if (this->getAvailablePeriodStart() == "" && this->getAvailablePeriodEnd() == "" && this->getUsedTimes() == 0)
			return true;

		tm tmAvaiStart = time.stringToTime(this->getAvailablePeriodStart());
		tm tmAvaiEnd = time.stringToTime(this->getAvailablePeriodEnd());

		// tm tmStart = time.stringToTime(start);
		// tm tmEnd = time.stringToTime(end);

		tm tmHouseStart = time.stringToTime(this->getStartDate());
		tm tmHouseEnd = time.stringToTime(this->getEndDate());

		if (time.compareSmallerTime(tmEnd, tmAvaiEnd) && time.compareBiggerTime(tmStart, tmAvaiStart) || this->getAvailablePeriodStart() == "" && this->getAvailablePeriodEnd() == "")
			if (time.compareSmallerTime(tmEnd, tmHouseStart) || time.compareBiggerTime(tmStart, tmHouseEnd))
				return true;
		return false;
	}

	// Count the days between the period for calculating consuming points in a period
	long countDays(string start, string end)
	{
		stringstream ss(start + "-" + end);
		int year, month, day;
		char hyphen;

		// Parse the first date into seconds
		ss >> year >> hyphen >> month >> hyphen >> day;
		struct tm starttm = {0, 0, 0, day, month - 1, year - 1900};
		time_t ttStart = mktime(&starttm);

		// Parse the second date into seconds
		ss >> hyphen >> year >> hyphen >> month >> hyphen >> day;
		struct tm endtm = {0, 0, 0, day, month - 1, year - 1900};
		time_t ttEnd = mktime(&endtm);

		// Find out the difference and divide it by 86400 to get the number of days
		return abs(ttEnd - ttStart) / 86400;
	}

	friend class Member;
};
