/**
 * @file    OASTime.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_TIME_H_
#define _OAS_TIME_H_

#include <time.h>

namespace oas
{

#define OAS_BILLION 1000000000

class Time
{
public:
    double asDouble();
    void update();

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
    const Time& operator+(const Time &other) const;
    const Time& operator-(const Time &other) const;

    Time(double floatingRepresentation);
    
private:

    struct timespec _time;
};

}

#endif

