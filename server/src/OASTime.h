/**
 * @file    OASTime.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_TIME_H_
#define _OAS_TIME_H_

#include <time.h>
#include <iostream>

namespace oas
{

#define OAS_BILLION 1000000000

class Time
{
public:
    void update();
    void reset();

    inline bool hasTime() const
    {
    	if (getSeconds() || getNanoseconds())
    		return true;
    	else
    		return false;
    }

    double asDouble() const;

    inline long int getSeconds() const
    {
    	return _time.tv_sec;
    }
    inline long int getNanoseconds() const
    {
    	return _time.tv_nsec;
    }

    Time& operator=(const Time &rhs);
    Time& operator+=(const Time &rhs);
    Time& operator-=(const Time &rhs);
    Time operator+(const Time &other) const;
    Time operator-(const Time &other) const;
    bool operator>(const Time &other) const;
    bool operator==(const Time &other) const;
    bool operator>=(const Time &other) const;

    Time(double floatingRepresentationInSeconds);
    Time();
    
private:

    struct timespec _time;
};

}

#endif

