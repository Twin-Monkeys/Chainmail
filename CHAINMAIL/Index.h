#pragma once

class Index 
{
public:
	/* constructor */
	Index() = default;
	Index(const int width, const int height, const int depth = 0);

	/* member function */
	bool operator==(const Index& rhs) const;
	void set(const int width, const int height, const int depth = 0);

	/* member variable */
	int width  = 0;
	int height = 0; 
	int depth  = 0;
};