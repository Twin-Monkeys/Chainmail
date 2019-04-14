#pragma once

class Vector3f
{
public:
	/* constructor */
	Vector3f() = default;
	explicit Vector3f(const float value);
	Vector3f(const float x, const float y, const float z);

	/* member function */
	void set(const float value);
	void set(const Vector3f& vec);
	void set(const float x, const float y, const float z);

	float getLength() const;
	Vector3f getUnit() const;
	void normalize();

	float dot(const float x, const float y, const float z) const;
	float dot(const Vector3f& vec) const;
	Vector3f cross(const float x, const float y, const float z) const;
	Vector3f cross(const Vector3f& vec) const;

	Vector3f operator+(const Vector3f& vec) const;
	Vector3f operator+(const float value) const;
	Vector3f operator-(const Vector3f& vec) const;
	Vector3f operator-(const float value) const;
	Vector3f operator-() const;
	Vector3f operator*(const Vector3f& vec) const;
	Vector3f operator*(const float value) const;
	Vector3f operator/(const Vector3f& vec) const;
	Vector3f operator/(const float value) const;

	bool operator==(const Vector3f& vec) const;

	Vector3f& operator+=(const Vector3f& rhs);
	Vector3f& operator+=(const float value);
	Vector3f& operator-=(const Vector3f& rhs);
	Vector3f& operator-=(const float value);
	Vector3f& operator*=(const Vector3f& rhs);
	Vector3f& operator*=(const float value);
	Vector3f& operator/=(const Vector3f& rhs);
	Vector3f& operator/=(const float value);

	/* member variable */
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};
