#include "Voxel.h"

/* constructor */
Voxel::Voxel(const Point3f& position) : position(position) {}

/* member function */
void Voxel::setPosition(const float x, const float y, const float z) 
{
	position.x = x;
	position.y = y;
	position.z = z;
}