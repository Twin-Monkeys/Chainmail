#include "MathUtility.h"
#include <cmath>

/* member function */
bool MathUtility::nearEqual(const float a, const float b, const float epsilon)
{
	if (fabsf(a - b) < epsilon)
		return true;

	return false;
}

bool MathUtility::greater(const float lower, const float upper, const float epsilon)
{
	if ((upper - lower) > epsilon)
		return true;

	return false;
}

float MathUtility::max(const float a, const float b)
{
	if (a > b)
		return a;

	return b;
}

float MathUtility::min(const float a, const float b)
{
	if (a < b)
		return a;

	return b;
}

float MathUtility::getDistance(const Point3f& p, const Point3f& q) 
{
	float retVal = square(p.x - q.x);
	retVal += square(p.y - q.y);
	retVal += square(p.z - q.z);

	return sqrtf(retVal);
}

float MathUtility::square(const float a)
{
	return (a * a);
}

float MathUtility::cube(const float a) 
{
	return (a * a * a);
}

float MathUtility::sqrt(const float a) 
{
	return sqrtf(a);
}