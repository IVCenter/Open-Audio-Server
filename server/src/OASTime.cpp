#include <OASTime.h>

using namespace oas;

void Time::update()
{
	clock_gettime(CLOCK_MONOTONIC, &_time);
}

double Time::asDouble()
{
	return (getSeconds() +  ((double) getNanoseconds() / OAS_BILLION));
}

Time& Time::operator=(const Time &rhs)
{
	if (this != &rhs)
	{
		_time.tv_sec = rhs.getSeconds();
		_time.tv_nsec = rhs.getNanoseconds();
	}

	return *this;
}

Time& Time::operator +=(const Time &rhs)
{
	long int nanoseconds = _time.tv_nsec + rhs.getNanoseconds();

	_time.tv_nsec = nanoseconds % OAS_BILLION;
	_time.tv_sec += rhs.getSeconds() + (nanoseconds / OAS_BILLION);

	return *this;
}

Time& Time::operator-=(const Time &rhs)
{
	if (_time.tv_nsec < rhs.getNanoseconds())
	{
		_time.tv_sec -= 1;
		_time.tv_nsec += OAS_BILLION;
	}

	_time.tv_nsec -= rhs.getNanoseconds();
	_time.tv_sec -= rhs.getSeconds();

	return *this;
}

const Time& Time::operator +(const Time &other) const
{
	return Time(*this) += other;
}

const Time& Time::operator -(const Time &other) const
{
	return Time(*this) -= other;
}

Time::Time(double floatingRepresentation)
{
	_time.tv_sec = (long int) floatingRepresentation;
	_time.tv_nsec = (long int) ((floatingRepresentation - _time.tv_sec) * OAS_BILLION);
}
