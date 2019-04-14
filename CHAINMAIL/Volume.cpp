#include "Volume.h"
#include <new>

/* constructor */
Volume::Volume(const int width, const int height, const int depth)
{
	setSize(width, height, depth);
}

Volume::Volume(const Volume& src)
{
	_copyFrom(src);
}

Volume::Volume(Volume&& src) noexcept
{
	width = src.width;
	height = src.height;
	depth = src.depth;
	_pBuffer = src._pBuffer;

	src.width = 0;
	src.height = 0;
	src.depth = 0;
	src._pBuffer = nullptr;
}

/* destructor */
Volume::~Volume()
{
	if (_pBuffer)
		_free();
}

/* member function */
void Volume::setSize(const int width, const int height, const int depth)
{
	if (_pBuffer)
		_free();

	this->width = width;
	this->height = height;
	this->depth = depth;

	_malloc();
}

int Volume::getWidth() const
{
	return width;
}

int Volume::getHeight() const
{
	return height;
}

int Volume::getDepth() const
{
	return depth;
}

Voxel* Volume::getPtr1D()
{
	return _pBuffer[0][0];
}

const Voxel* Volume::getPtr1D() const 
{
	return const_cast<Voxel*>(_pBuffer[0][0]);
}

Voxel*** Volume::getPtr3D()
{
	return _pBuffer;
}

const Voxel*** Volume::getPtr3D() const
{
	return const_cast<const Voxel***>(_pBuffer);
}

Volume& Volume::operator=(const Volume& rhs)
{
	if (this == &rhs)
		return *this;

	_free();
	_copyFrom(rhs);

	return *this;
}

Volume& Volume::operator=(Volume&& rhs) noexcept
{
	if (this == &rhs)
		return *this;

	_free();

	width = rhs.width;
	height = rhs.height;
	depth = rhs.depth;
	_pBuffer = rhs._pBuffer;

	rhs.width = 0;
	rhs.height = 0;
	rhs.depth = 0;
	rhs._pBuffer = nullptr;

	return *this;
}

void Volume::_malloc()
{
	_pBuffer = new Voxel**[depth];

	for (int i = 0; i < depth; ++i)
	{
		int idx = 0;

		Voxel* placeholder = new Voxel[height * width];
		_pBuffer[i] = new Voxel*[height];

		for (int j = 0; j < height; ++j)
		{
			_pBuffer[i][j] = (placeholder + idx);
			idx += width;
		}
	}
}

void Volume::_free()
{
	for (int i = 0; i < depth; ++i)
		delete[] _pBuffer[i];

	delete[] _pBuffer;

	_pBuffer = nullptr;
}

void Volume::_copyFrom(const Volume& src)
{
	width = src.width;
	height = src.height;
	depth = src.depth;

	_malloc();

	for (int i = 0; i < depth; ++i)
		for (int j = 0; j < height; ++j)
			for (int k = 0; k < width; ++k)
				_pBuffer[i][j][k] = src._pBuffer[i][j][k];
}