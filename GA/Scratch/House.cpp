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
	: houseRatingScrore(0), consumingPoints(0), usedTimes(0), requiredMinOccupierRating(0){}

	House(string location, string descripton)
	: location(location), descripton(descripton), usedTimes(0), requiredMinOccupierRating(0), houseRatingScrore(0){}
	

	string getLocation()
	{
		return location;
	}

	void setLocation(string location)
	{
		location = location;
	}

	string getDescription()
	{
		return descripton;
	}

	void setDescription(string descripton)
	{
		descripton = descripton;
	}

	double getHouseRatingScore()
	{
		return houseRatingScrore;
	}

	void setHouseRatingScore(double houseRatingScore)
	{
		houseRatingScrore = houseRatingScore;
	}

	string getStartDate()
	{
		return startDate;
	}

	void setStartDate(string startDate)
	{
		startDate = startDate;
	}

	string getEndDate()
	{
		return endDate;
	}

	void setEndDate(string endDate)
	{
		endDate = endDate;
	}

	void setAvailablePeriodStart(string availablePeriodStart)
	{
		availablePeriodStart = availablePeriodStart;
	}

	string getAvailablePeriodEnd()
	{
		return availablePeriodEnd;
	}

	void setAvailablePeriodEnd(string availablePeriodEnd)
	{
		availablePeriodEnd = availablePeriodEnd;
	}

	string getAvailablePeriodStart()
	{
		return availablePeriodStart;
	}

	double getConsumingPoints()
	{
		return consumingPoints;
	}

	void newConsumingPoints(){
		setConsumingPoints(getUsedTimes()/getUsedDays());
	}

	void setConsumingPoints(double consumingPoints)
	{
		consumingPoints = consumingPoints;
	}

	long getUsedTimes()
	{
		return usedTimes;
	}

	void setUsedTimes()
	{
		usedTimes++;
	}

	double getRequiredMinOccupierRating()
	{
		return requiredMinOccupierRating;
	}

	void setRequiredMinOccupierRating(double requiredMinOccupier)
	{
		requiredMinOccupierRating = requiredMinOccupier;
	}

	void setUsedDays(long usedDays)
	{
		usedDays = usedDays;
	}

	long getUsedDays()
	{
		return usedDays;
	}

	void setComments(vector<string> comments)
	{
		comments = comments;
	}

	vector<string> getComments()
	{
		return comments;
	}

	void setRequests(vector<Request *> requests)
	{
		requests = requests;
	}

	vector<Request *> getRequests()
	{
		return requests;
	}

	// Check if the house if free between start and end
	bool isFree(string start, string end)
	{
		Time time = Time();
		tm tmStart = time.stringToTime(start);
		tm tmEnd = time.stringToTime(end);

		if (time.compareBiggerTime(tmStart, tmEnd))
			return false;

		if (getAvailablePeriodStart() == "" && getAvailablePeriodEnd() == "" && getUsedTimes() == 0)
			return true;

		tm tmAvaiStart = time.stringToTime(getAvailablePeriodStart());
		tm tmAvaiEnd = time.stringToTime(getAvailablePeriodEnd());

		// tm tmStart = time.stringToTime(start);
		// tm tmEnd = time.stringToTime(end);

		tm tmHouseStart = time.stringToTime(getStartDate());
		tm tmHouseEnd = time.stringToTime(getEndDate());

		if (time.compareSmallerTime(tmEnd, tmAvaiEnd) && time.compareBiggerTime(tmStart, tmAvaiStart) || getAvailablePeriodStart() == "" && getAvailablePeriodEnd() == "")
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
