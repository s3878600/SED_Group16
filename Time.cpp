#include <iostream>
#include <ctime>

using namespace std;

class Time {
public:
    // Convert string to tm datatype
    static tm stringToTime(const string& str) {
        tm tmStr;
        std::sscanf(str.c_str(), "%4d/%2d/%2d", &tmStr.tm_year, &tmStr.tm_mon, &tmStr.tm_mday);
        // Adjust for zero-based month and day fields
        tmStr.tm_mon -= 1;
        tmStr.tm_mday -= 1;
        return tmStr;
    }

    // Compare two tm datatypes - returns true if t1 is smaller than t2
    static bool compareSmallerTime(const tm& t1, const tm& t2) {
        if (t1.tm_year < t2.tm_year) return true;
        else if (t1.tm_year == t2.tm_year) {
            if (t1.tm_mon < t2.tm_mon) return true;
            else if (t1.tm_mon == t2.tm_mon) {
                if (t1.tm_mday < t2.tm_mday) return true;
            }
        }
        return false;
    }

    // Compare two tm datatypes - returns true if t1 is bigger than t2
    static bool compareBiggerTime(const tm& t1, const tm& t2) {
        if (t1.tm_year > t2.tm_year) return true;
        else if (t1.tm_year == t2.tm_year) {
            if (t1.tm_mon > t2.tm_mon) return true;
            else if (t1.tm_mon == t2.tm_mon) {
                if (t1.tm_mday > t2.tm_mday) return true;
            }
        }
        return false;
    }
    friend class Member;
    friend class House;
    friend class System;
};