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
	string startPeriod;
	string endPeriod;
	double consumingPoints; 
	double MinReqOccupierRating;
	long spendTimes;
	long spendDays;
	vector<string> comments;
	vector<Request *> requests;


public:
	House()
	{
		this->houseRatingScrore = 0;
		this->consumingPoints = 0;
		this->spendTimes = 0;
		this->MinReqOccupierRating = 0;
	};

	House(string location, string descripton)
	{
		this->location = location;
		this->descripton = descripton;
		this->spendTimes = 0;
		this->MinReqOccupierRating = 0;
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

	void setStartPeriod(string startPeriod)
	{
		this->startPeriod = startPeriod;
	}

	string getEndPeriod()
	{
		return this->endPeriod;
	}

	void setEndPeriod(string endPeriod)
	{
		this->endPeriod = endPeriod;
	}

	string getStartPeriod()
	{
		return this->startPeriod;
	}

	double getConsumingPoints()
	{
		return this->consumingPoints;
	}

	void setConsumingPoints(double consumingPoints)
	{
		this->consumingPoints = consumingPoints;
	}

	long getSpendTimes()
	{
		return this->spendTimes;
	}

	void setSpendTimes()
	{
		this->spendTimes++;
	}

	double getMinReqOccupierRating()
	{
		return this->MinReqOccupierRating;
	}

	void setMinReqOccupierRating(double MinOccupier)
	{
		this->MinReqOccupierRating = MinOccupier;
	}

	void setSpendDays(long spendDays)
	{
		this->spendDays = spendDays;
	}

	long getSpendDays()
	{
		return this->spendDays;
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



	bool available(string start, string end)
	{
		Time time = Time();
		tm tmStart = time.stringToTime(start);
		tm tmEnd = time.stringToTime(end);

		if (time.compareBiggerTime(tmStart, tmEnd))
			return false;

		if (this->getStartPeriod() == "" && this->getEndPeriod() == "" && this->getSpendTimes() == 0)
			return true;

		tm tmAvaiStart = time.stringToTime(this->getStartPeriod());
		tm tmAvaiEnd = time.stringToTime(this->getEndPeriod());


		tm tmHouseStart = time.stringToTime(this->getStartDate());
		tm tmHouseEnd = time.stringToTime(this->getEndDate());

		if (time.compareSmallerTime(tmEnd, tmAvaiEnd) && time.compareBiggerTime(tmStart, tmAvaiStart) || this->getStartPeriod() == "" && this->getEndPeriod() == "")
			if (time.compareSmallerTime(tmEnd, tmHouseStart) || time.compareBiggerTime(tmStart, tmHouseEnd))
				return true;
		return false;
	}


	long countDays(string start, string end)
	{
		stringstream ss(start + "-" + end);
		int year, month, day;
		char hyphen;


		ss >> year >> hyphen >> month >> hyphen >> day;
		struct tm starttm = {0, 0, 0, day, month - 1, year - 1900};
		time_t ttStart = mktime(&starttm);

	
		ss >> hyphen >> year >> hyphen >> month >> hyphen >> day;
		struct tm endtm = {0, 0, 0, day, month - 1, year - 1900};
		time_t ttEnd = mktime(&endtm);


		return abs(ttEnd - ttStart) / 86400;
	}

	friend class Member;
};
