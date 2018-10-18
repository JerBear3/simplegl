#include <math.h>
#include "../math.hpp"

Quaternion::Quaternion()
{
	idt();
}

Quaternion::Quaternion(const Vector3& axis, float rad)
{
	set(axis, rad);
}

Quaternion& Quaternion::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	return *this;
}

Quaternion& Quaternion::set(const Vector3& axis, float rad)
{
	float d = axis.len2();
	if(d == 0.f) return idt();
	d = sqrt(d);
	
	float ang = rad < 0.f ? PI2 - fmodf(-rad, PI2) : fmodf(rad, PI2);
	float fsin = sin(ang / 2.f);
	float fcos = cos(ang / 2.f);
	
	return set(axis.x * fsin / d, axis.y * fsin / d, axis.z * fsin / d, fcos).nor();
}

Quaternion& Quaternion::idt()
{
	return set(0.f, 0.f, 0.f, 1.f);
}

Quaternion& Quaternion::nor()
{
	float len = x * x + y * y + z * z + w * w;
	if(len != 0.f && len != 1.f)
	{
		len = sqrt(len);
		w /= len;
		x /= len;
		y /= len;
		z /= len;
	}
	
	return *this;
}
