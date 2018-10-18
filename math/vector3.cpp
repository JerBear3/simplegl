#include <math.h>
#include "../math.hpp"

Vector3 Vector3::operator*(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator*(const Matrix4& mat) const
{
	return Vector3(x * mat.val[M00] + y * mat.val[M01] + z * mat.val[M02] + mat.val[M03],
			x * mat.val[M10] + y * mat.val[M11] + z * mat.val[M12] + mat.val[M13],
			x * mat.val[M20] + y * mat.val[M21] + z * mat.val[M22] + mat.val[M23]);
}

Vector3 Vector3::operator*(const Quaternion& quat) const
{
	Matrix4 mat(quat);
	return *this * mat;
}

Vector3& Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3& Vector3::operator*=(const Matrix4& mat)
{
	Vector3 b = *this * mat;
	return *this = b;
}

Vector3& Vector3::operator*=(const Quaternion& quat)
{
	Matrix4 mat(quat);
	return *this *= mat;
}

Vector3 Vector3::operator+(const Vector3& b) const
{
	Vector3 c = *this;
	return c += b;
}

Vector3 Vector3::operator-(const Vector3& b) const
{
	Vector3 c = *this;
	return c -= b;
}

Vector3& Vector3::operator+=(const Vector3& b)
{
	x += b.x;
	y += b.y;
	z += b.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& b)
{
	x -= b.x;
	y -= b.y;
	z -= b.z;
	return *this;
}

Vector3& Vector3::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

Vector3& Vector3::nor()
{
	float len = len2();
	if(len != 0.f && len != 1.f)
	{
		len = sqrt(len);
		*this *= 1.f / len;
	}
	
	return *this;
}

Vector3& Vector3::crs(const Vector3& b)
{
	Vector3 c;
	c.x = y * b.z - z * b.y;
	c.y = z * b.x - x * b.z;
	c.z = x * b.y - y * b.x;
	return *this = c;
}

Vector3& Vector3::rotate(const Vector3& vec, float rad)
{
	Quaternion quat(vec, rad);
	return *this *= quat;
}

float Vector3::len2() const
{
	return x * x + y * y + z * z;
}

float Vector3::len() const
{
	return sqrt(len2());
}

float Vector3::dot(const Vector3& b) const
{
	return x * b.x + y * b.y + z * b.z;
}
