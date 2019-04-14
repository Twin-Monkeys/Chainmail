#pragma once

#include "Voxel.h"

class Volume
{
public:
	/* constructor & destructor */
	Volume(const int width, const int height, const int depth);
	Volume(const Volume& src);
	Volume(Volume&& src) noexcept;
	~Volume();

	/* member function */
	void setSize(const int width, const int height, const int depth);

	int getWidth() const;
	int getHeight() const;
	int getDepth() const;

	Voxel* getPtr1D();
	const Voxel* getPtr1D() const;
	Voxel*** getPtr3D();
	const Voxel*** getPtr3D() const;

	Volume& operator=(const Volume& rhs);
	Volume& operator=(Volume&& rhs) noexcept;

private:
	/* member function */
	void _malloc();
	void _free();
	void _copyFrom(const Volume& src);

	/* member variable */
	int width;
	int height;
	int depth;

	Voxel*** _pBuffer = nullptr;
};