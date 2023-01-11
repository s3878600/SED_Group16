#include<iostream>
#include<stdio.h>

using namespace std;


class Time{
    Time time(){}


    tm stringToTime(string str){
        const char *cstr = str.c_str();
        tm tmStr;
        sscanf(cstr,"%4d/%2d/%2d",&tmStr.tm_year,&tmStr.tm_mon,&tmStr.tm_mday);
        return tmStr;
    }


    bool compareSmallerTime(tm t1, tm t2){
        if(t1.tm_year>t2.tm_year) return false;
        else if(t1.tm_year=t2.tm_year){
            if(t1.tm_mon>t2.tm_mon) return false;
            else if(t1.tm_mon=t2.tm_mon){
                if(t1.tm_mday>t2.tm_mday) return false;
            }
        }
        return true;
    }



    bool compareBiggerTime(tm t1, tm t2){
        if(t1.tm_year<t2.tm_year) return false;
        else if(t1.tm_year=t2.tm_year){
            if(t1.tm_mon<t2.tm_mon) return false;
            else if(t1.tm_mon=t2.tm_mon){
                if(t1.tm_mday<t2.tm_mday) return false;
            }
        }
        return true;
    }

    friend class Member;
    friend class House;
    friend class System;
};