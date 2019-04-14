#pragma once

class Plasticity 
{
public:
	/* member function */
	Plasticity& operator*=(const float value);

	/* member variable */
	float x = 1.f;
	float y = 1.f;
	float z = 1.f;
};