#include "Index.h"

/* constructor */
Index::Index(const int width, const int height, const int depth)
{
	set(width, height, depth);
}

/* member function */
bool Index::operator==(const Index& rhs) const
{
	if ((width == rhs.width) &&
		(height == rhs.height) &&
		(depth == rhs.depth))
		return true;

	return false;
}

void Index::set(const int width, const int height, const int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
}