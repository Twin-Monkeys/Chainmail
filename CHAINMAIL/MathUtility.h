#pragma once

#include "Point3f.h"

class MathUtility 
{
public:
	/* constructor & destructor */
	MathUtility() = delete;

	/* member function */
	static bool nearEqual(const float a, const float b, const float epsilon = 1e-5f);
	static bool greater(const float lower, const float upper, const float epsilon = 1e-5f);

	static float max(const float a, const float b);
	static float min(const float a, const float b);

	static float getDistance(const Point3f& p, const Point3f& q);
	
	static float square(const float a);
	static float cube(const float a);
	static float sqrt(const float a);
};