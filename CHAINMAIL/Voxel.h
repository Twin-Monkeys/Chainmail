#pragma once

#include "Point3f.h"

class Voxel 
{
public:
	/* constructor */
	Voxel() = default;
	explicit Voxel(const Point3f& position);

	/* member function */
	void setPosition(const float x, const float y, const float z);

	/* member variable */
	Point3f position;

	Voxel* left   = nullptr;
	Voxel* right  = nullptr;
	Voxel* top    = nullptr;
	Voxel* bottom = nullptr;
	Voxel* front  = nullptr;
	Voxel* back   = nullptr;
};