#include "Vector3f.h"
#include "MathUtility.h"

/* constructor */
Vector3f::Vector3f(const float value)
: x(value), y(value), z(value)
{}

Vector3f::Vector3f(const float x, const float y, const float z)
: x(x), y(y), z(z)
{}

/* member function */
void Vector3f::set(const float value)
{
	this->x = value;
	this->y = value;
	this->z = value;
}

void Vector3f::set(const Vector3f& vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

void Vector3f::set(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3f::getLength() const
{
	float squaredVal = 0.f;

	squaredVal += MathUtility::square(x);
	squaredVal += MathUtility::square(y);
	squaredVal += MathUtility::square(z);

	return MathUtility::sqrt(squaredVal);
}

Vector3f Vector3f::getUnit() const
{
	return (*this / getLength());
}

void Vector3f::normalize()
{
	(*this /= getLength());
}

float Vector3f::dot(const float x, const float y, const float z) const
{
	float xVal = (this->x * x);
	float yVal = (this->y * y);
	float zVal = (this->z * z);

	return (xVal + yVal + zVal);
}

float Vector3f::dot(const Vector3f& vec) const
{
	float xVal = (this->x * vec.x);
	float yVal = (this->y * vec.y);
	float zVal = (this->z * vec.z);

	return (xVal + yVal + zVal);
}

Vector3f Vector3f::cross(const float x, const float y, const float z) const
{
	float xVal = (this->y * z) - (this->z * y);
	float yVal = (this->z * x) - (this->x * z);
	float zVal = (this->x * y) - (this->y * x);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::cross(const Vector3f& vec) const
{
	float xVal = (this->y * vec.z) - (this->z * vec.y);
	float yVal = (this->z * vec.x) - (this->x * vec.z);
	float zVal = (this->x * vec.y) - (this->y * vec.x);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator+(const Vector3f& vec) const
{
	float xVal = (x + vec.x);
	float yVal = (y + vec.y);
	float zVal = (z + vec.z);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator+(const float value) const
{
	float xVal = (x + value);
	float yVal = (y + value);
	float zVal = (z + value);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator-(const Vector3f& vec) const
{
	float xVal = (x - vec.x);
	float yVal = (y - vec.y);
	float zVal = (z - vec.z);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator-(const float value) const
{
	float xVal = (x - value);
	float yVal = (y - value);
	float zVal = (z - value);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator-() const
{
	return Vector3f(-x, -y, -z);
}

Vector3f Vector3f::operator*(const Vector3f& vec) const
{
	float xVal = (x * vec.x);
	float yVal = (y * vec.y);
	float zVal = (z * vec.z);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator*(const float value) const
{
	float xVal = (x * value);
	float yVal = (y * value);
	float zVal = (z * value);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator/(const Vector3f& vec) const
{
	float xVal = (x / vec.x);
	float yVal = (y / vec.y);
	float zVal = (z / vec.z);

	return Vector3f(xVal, yVal, zVal);
}

Vector3f Vector3f::operator/(const float value) const
{
	const float INV_VAL = (1.f / value);

	float xVal = (x * INV_VAL);
	float yVal = (y * INV_VAL);
	float zVal = (z * INV_VAL);

	return Vector3f(xVal, yVal, zVal);
}

bool Vector3f::operator==(const Vector3f& vec) const 
{
	if (MathUtility::nearEqual(x, vec.x) &&
		MathUtility::nearEqual(y, vec.y) &&
		MathUtility::nearEqual(z, vec.z))
		return true;

	return false;
}

Vector3f& Vector3f::operator+=(const Vector3f& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector3f& Vector3f::operator+=(const float value)
{
	x += value;
	y += value;
	z += value;

	return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vector3f& Vector3f::operator-=(const float value)
{
	x -= value;
	y -= value;
	z -= value;

	return *this;
}

Vector3f& Vector3f::operator*=(const Vector3f& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;

	return *this;
}

Vector3f& Vector3f::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

Vector3f& Vector3f::operator/=(const Vector3f& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;

	return *this;
}

Vector3f& Vector3f::operator/=(const float value)
{
	const float INV_VAL = (1.f / value);

	x *= INV_VAL;
	y *= INV_VAL;
	z *= INV_VAL;

	return *this;
}