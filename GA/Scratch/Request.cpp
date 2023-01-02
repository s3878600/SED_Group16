#include <iostream>
#include <stdio.h>

using namespace std;

// Request class
class Request
{
private:
    string start;
    string end;
    string requestUsername;
    string status;

public:
    Request(): status("pending"){}

    //setter getter for private atttribute
    string getRequestUsername()
    {
        return requestUsername;
    }

    void setRequestUsername(string requestUsername)
    {
        requestUsername = requestUsername;
    }

    string getStart()
    {
        return start;
    }

    void setStart(string start)
    {
        start = start;
    }

    string getEnd()
    {
        return end;
    }

    void setEnd(string end)
    {
        end = end;
    }

    string getStatus()
    {
        return status;
    }

    void setStatus(string status)
    {
        status = status;
    }
};