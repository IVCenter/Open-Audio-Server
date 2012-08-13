/**
 * @file    Time.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_TIME_H_
#define _OAS_TIME_H_

#include <time.h>

namespace oasclient
{

#define OAS_BILLION 1000000000

class Time
{
public:

	/**
	 * ClockType defines the types of clocks that can be recorded.
	 *  CLOCK_MONOTONIC: Fast access clock that cannot be modified by programs, monotonic since
	 *  				 some unspecified starting point. Best used for timing intervals.
	 *  CLOCK_REALTIME: System-wide realtime clock that can be set with appropriate privileges.
	 */
	enum ClockType {
		OAS_CLOCK_MONOTONIC = CLOCK_MONOTONIC,
		OAS_CLOCK_REALTIME = CLOCK_REALTIME
	};

	/**
	 * @brief Update with the value of the current time as recorded by the specified clock
	 */
    inline void update(const ClockType clockToUse)
    {
        clock_gettime(clockToUse, &_time);
    }

    /**
     * @brief Reset the value in the clock so that it no longer records any valid time.
     *
     */
    inline void reset()
    {
    	_time.tv_sec = _time.tv_nsec = 0;
    }

    /**
     * @brief Check if this time object records a valid time
     */
    inline bool hasTime() const
    {
    	if (getSeconds() || getNanoseconds())
    		return true;
    	else
    		return false;
    }

    /**
     * @brief Return the stored value as a double precision floating point number
     */
    double asDouble() const;

    /**
     * @brief Return the stored value as a struct timespec
     */
    inline const struct timespec getTime() const
    {
    	return _time;
    }

    /**
     * @brief Return the seconds portion of the stored value
     */
    inline const long int getSeconds() const
    {
    	return _time.tv_sec;
    }

    /**
     * @brief Return the nanoseconds portion of the stored value
     */
    inline const long int getNanoseconds() const
    {
    	return _time.tv_nsec;
    }

    /**
     * @brief Overloaded assignment operator
     */
    Time& operator=(const Time &rhs);

    /**
     * @brief Overloaded += operator
     */
    Time& operator+=(const Time &rhs);

    /**
     * @brief Overloaded -= operator
     */
    Time& operator-=(const Time &rhs);

    /**
     * @brief Overloaded addition operator
     */
    Time operator+(const Time &other) const;

    /**
     * @brief Overloaded subtraction operator
     */
    Time operator-(const Time &other) const;

    /**
     * @brief Overloaded greater-than comparison operator
     */
    bool operator>(const Time &other) const;

    /**
     * @brief Overloaded equivalence comparison operator
     */
    bool operator==(const Time &other) const;

    /**
     * @brief Overloaded greater-than-or-equal-to comparison operator
     */
    bool operator>=(const Time &other) const;

    /**
     * @brief Constructor that sets the internal value directly
     */
    Time(long int seconds, long int nanoseconds);

    /**
     * @brief Constructor that initializes based on a floating point representation, in seconds.
     */
    Time(double floatingRepresentationInSeconds);
    
    /**
     * @brief Constructor that sets an empty time.
     */
    Time();
private:

    struct timespec _time;
};

}

#endif

