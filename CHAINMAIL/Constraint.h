#pragma once

#include "Range.h"

class Constraint 
{
public:
	/* member variable */
	Rangef dX = { 1.f, 1.f };
	Rangef dY = { 1.f, 1.f };
	Rangef dZ = { 1.f, 1.f };

	float shearX = 0.f;
	float shearY = 0.f;
	float shearZ = 0.f;
};