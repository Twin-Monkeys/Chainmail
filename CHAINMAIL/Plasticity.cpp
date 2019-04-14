#include "Plasticity.h"

/* member function */
Plasticity& Plasticity::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}