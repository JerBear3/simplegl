#include <math.h>
#include "../math.hpp"

Matrix4::Matrix4()
{
	idt();
}

Matrix4::Matrix4(const Quaternion& quat)
{
	*this = quat;
}

Matrix4::Matrix4(const SimpleTransform& st)
{
	*this = st;
}

Matrix4::Matrix4(const Vector3& position, const Vector3& direction, const Vector3& up)
{
	setView(position, direction, up);
}

Matrix4::Matrix4(float near, float far, float fovy, float aspectRatio)
{
	setPerspective(near, far, fovy, aspectRatio);
}

float& Matrix4::operator[](int i)
{
	return val[i]; 
}

Matrix4& Matrix4::operator=(const Quaternion& quat)
{
	float xs = quat.x * 2.f, ys = quat.y * 2.f, zs = quat.z * 2.f;
	float wx = quat.w * xs,  wy = quat.w * ys,  wz = quat.w * zs;
	float xx = quat.x * xs,  xy = quat.x * ys,  xz = quat.x * zs;
	float yy = quat.y * ys,  yz = quat.y * zs,  zz = quat.z * zs;
	
	val[M00] = 1.f - yy - zz; val[M01] = xy - wz;       val[M02] = xz + wy;       val[M03] = 0.f;
	val[M10] = xy + wz;       val[M11] = 1.f - xx - zz; val[M12] = yz - wx;       val[M13] = 0.f;
	val[M20] = xz - wy;       val[M21] = yz + wx;       val[M22] = 1.f - xx - yy; val[M23] = 0.f;
	val[M30] = 0.f;           val[M31] = 0.f;           val[M32] = 0.f;           val[M33] = 1.f;
	
	return *this;
}

Matrix4& Matrix4::operator=(const SimpleTransform& st)
{
	setTranslation(st.translation);
	*this *= st.rotation;
	*this *= st.scale;
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& b) const
{
	Matrix4 ret;
	ret.val[M00] = val[M00] * b.val[M00] + val[M01] * b.val[M10] + val[M02] * b.val[M20] + val[M03] * b.val[M30];
	ret.val[M01] = val[M00] * b.val[M01] + val[M01] * b.val[M11] + val[M02] * b.val[M21] + val[M03] * b.val[M31];
	ret.val[M02] = val[M00] * b.val[M02] + val[M01] * b.val[M12] + val[M02] * b.val[M22] + val[M03] * b.val[M32];
	ret.val[M03] = val[M00] * b.val[M03] + val[M01] * b.val[M13] + val[M02] * b.val[M23] + val[M03] * b.val[M33];
	ret.val[M10] = val[M10] * b.val[M00] + val[M11] * b.val[M10] + val[M12] * b.val[M20] + val[M13] * b.val[M30];
	ret.val[M11] = val[M10] * b.val[M01] + val[M11] * b.val[M11] + val[M12] * b.val[M21] + val[M13] * b.val[M31];
	ret.val[M12] = val[M10] * b.val[M02] + val[M11] * b.val[M12] + val[M12] * b.val[M22] + val[M13] * b.val[M32];
	ret.val[M13] = val[M10] * b.val[M03] + val[M11] * b.val[M13] + val[M12] * b.val[M23] + val[M13] * b.val[M33];
	ret.val[M20] = val[M20] * b.val[M00] + val[M21] * b.val[M10] + val[M22] * b.val[M20] + val[M23] * b.val[M30];
	ret.val[M21] = val[M20] * b.val[M01] + val[M21] * b.val[M11] + val[M22] * b.val[M21] + val[M23] * b.val[M31];
	ret.val[M22] = val[M20] * b.val[M02] + val[M21] * b.val[M12] + val[M22] * b.val[M22] + val[M23] * b.val[M32];
	ret.val[M23] = val[M20] * b.val[M03] + val[M21] * b.val[M13] + val[M22] * b.val[M23] + val[M23] * b.val[M33];
	ret.val[M30] = val[M30] * b.val[M00] + val[M31] * b.val[M10] + val[M32] * b.val[M20] + val[M33] * b.val[M30];
	ret.val[M31] = val[M30] * b.val[M01] + val[M31] * b.val[M11] + val[M32] * b.val[M21] + val[M33] * b.val[M31];
	ret.val[M32] = val[M30] * b.val[M02] + val[M31] * b.val[M12] + val[M32] * b.val[M22] + val[M33] * b.val[M32];
	ret.val[M33] = val[M30] * b.val[M03] + val[M31] * b.val[M13] + val[M32] * b.val[M23] + val[M33] * b.val[M33];
	
	return ret;
}

Matrix4 Matrix4::operator*(const Quaternion& quat) const
{
	Matrix4 b(quat);
	return *this * b;
}

Matrix4 Matrix4::operator*(const Vector3& vec) const
{
	Matrix4 b;
	return *this * b.setScale(vec);
}

Matrix4 Matrix4::operator*(const SimpleTransform& st) const
{
	Matrix4 b(st);
	return *this * b;
}

Matrix4& Matrix4::operator*=(const Matrix4& b)
{
	Matrix4 c = *this * b;
	return *this = c;
}

Matrix4& Matrix4::operator*=(const Quaternion& quat)
{
	Matrix4 b(quat);
	return *this *= b;
}

Matrix4& Matrix4::operator*=(const Vector3& vec)
{
	Matrix4 b;
	return *this *= b.setScale(vec);
}

Matrix4& Matrix4::operator*=(const SimpleTransform& st)
{
	Matrix4 b(st);
	return *this *= b;
}

Matrix4 Matrix4::operator+(const Vector3& vec) const
{
	Matrix4 b;
	return *this * b.setTranslation(vec);
}

Matrix4 Matrix4::operator-(const Vector3& vec) const
{
	Matrix4 b;
	return *this * b.setTranslation(Vector3(vec) *= -1.f);
}

Matrix4& Matrix4::operator+=(const Vector3& vec)
{
	Matrix4 b;
	return *this *= b.setTranslation(vec);
}

Matrix4& Matrix4::operator-=(const Vector3& vec)
{
	Matrix4 b;
	return *this *= b.setTranslation(Vector3(vec) *= -1.f);
}

Matrix4& Matrix4::setTranslation(const Vector3& vec)
{
	val[M00] = 1.f; val[M01] = 0.f; val[M02] = 0.f;  val[M03] = vec.x;
	val[M10] = 0.f; val[M11] = 1.f; val[M12] = 0.f;  val[M13] = vec.y;
	val[M20] = 0.f; val[M21] = 0.f; val[M22] = 1.f;  val[M23] = vec.z;
	val[M30] = 0.f; val[M31] = 0.f; val[M32] = 0.f;  val[M33] = 1.f;
	return *this;
}

Matrix4& Matrix4::setScale(const Vector3& vec)
{
	val[M00] = vec.x; val[M01] = 0.f;   val[M02] = 0.f;   val[M03] = 0.f;
	val[M10] = 0.f;   val[M11] = vec.y; val[M12] = 0.f;   val[M13] = 0.f;
	val[M20] = 0.f;   val[M21] = 0.f;   val[M22] = vec.z; val[M23] = 0.f;
	val[M30] = 0.f;   val[M31] = 0.f;   val[M32] = 0.f;   val[M33] = 1.f;
	return *this;
}

Matrix4& Matrix4::setPerspective(float near, float far, float fovy, float aspectRatio)
{
	float fd = (1.f / tan(fovy / 2.f));
	float a1 = (far + near) / (near - far);
	float a2 = (2.f * far * near) / (near - far);
	
	val[M00] = fd / aspectRatio; val[M01] = 0.f; val[M02] =  0.f; val[M03] = 0.f;
	val[M10] = 0.f;              val[M11] = fd;  val[M12] =  0.f; val[M13] = 0.f;
	val[M20] = 0.f;              val[M21] = 0.f; val[M22] =  a1;  val[M23] = a2;
	val[M30] = 0.f;              val[M31] = 0.f; val[M32] = -1.f; val[M33] = 0.f;
	return *this;
}

Matrix4& Matrix4::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	val[M00] = 2.f / (right - left); val[M01] = 0;                    val[M02] =  0;                  val[M03] = -(right + left) / (right - left);
	val[M10] = 0;                    val[M11] = 2.f / (top - bottom); val[M12] =  0;                  val[M13] = -(top + bottom) / (top - bottom);
	val[M20] = 0;                    val[M21] = 0;                    val[M22] = -2.f / (far - near); val[M23] = -(far + near)   / (far - near);
	val[M30] = 0;                    val[M31] = 0;                    val[M32] =  0;                  val[M33] = 1;
	
	return *this;
}

Matrix4& Matrix4::setView(const Vector3& position, const Vector3& direction, const Vector3& up)
{
	Vector3 z(direction); //direction
	z.nor();
	Vector3 x(z); //right
	x.crs(up).nor();
	Vector3 y(x); //corrected up
	y.crs(z).nor();
	
	val[M00] =  x.x; val[M01] =  x.y; val[M02] =  x.z; val[M03] = 0.f;
	val[M10] =  y.x; val[M11] =  y.y; val[M12] =  y.z; val[M13] = 0.f;
	val[M20] = -z.x; val[M21] = -z.y; val[M22] = -z.z; val[M23] = 0.f;
	val[M30] =  0.f; val[M31] =  0.f; val[M32] =  0.f; val[M33] = 1.f;
	return *this -= position;
}

Matrix4& Matrix4::calculateNormal(Matrix4& out) const
{
	out = *this;
	out.val[M03] = 0.f;
	out.val[M13] = 0.f;
	out.val[M23] = 0.f;
	return out.inv().tra();
}

Matrix4& Matrix4::idt()
{
	val[M00] = 1.f; val[M01] = 0.f; val[M02] = 0.f;  val[M03] = 0.f;
	val[M10] = 0.f; val[M11] = 1.f; val[M12] = 0.f;  val[M13] = 0.f;
	val[M20] = 0.f; val[M21] = 0.f; val[M22] = 1.f;  val[M23] = 0.f;
	val[M30] = 0.f; val[M31] = 0.f; val[M32] = 0.f;  val[M33] = 1.f;
	
	return *this;
}

Matrix4& Matrix4::inv(bool* success)
{
	float rdet = det();
	if(rdet == 0.f)
	{
		if(success != nullptr)
			*success = false;
		
		return *this;
	}
	
	Matrix4 b;
	b.val[M00] = val[M12] * val[M23] * val[M31] - val[M13] * val[M22] * val[M31] + val[M13] * val[M21] * val[M32] -
			val[M11] * val[M23] * val[M32] - val[M12] * val[M21] * val[M33] + val[M11] * val[M22] * val[M33];
	b.val[M01] = val[M03] * val[M22] * val[M31] - val[M02] * val[M23] * val[M31] - val[M03] * val[M21] * val[M32] +
			val[M01] * val[M23] * val[M32] + val[M02] * val[M21] * val[M33] - val[M01] * val[M22] * val[M33];
	b.val[M02] = val[M02] * val[M13] * val[M31] - val[M03] * val[M12] * val[M31] + val[M03] * val[M11] * val[M32] -
			val[M01] * val[M13] * val[M32] - val[M02] * val[M11] * val[M33] + val[M01] * val[M12] * val[M33];
	b.val[M03] = val[M03] * val[M12] * val[M21] - val[M02] * val[M13] * val[M21] - val[M03] * val[M11] * val[M22] +
			val[M01] * val[M13] * val[M22] + val[M02] * val[M11] * val[M23] - val[M01] * val[M12] * val[M23];
	b.val[M10] = val[M13] * val[M22] * val[M30] - val[M12] * val[M23] * val[M30] - val[M13] * val[M20] * val[M32] +
			val[M10] * val[M23] * val[M32] + val[M12] * val[M20] * val[M33] - val[M10] * val[M22] * val[M33];
	b.val[M11] = val[M02] * val[M23] * val[M30] - val[M03] * val[M22] * val[M30] + val[M03] * val[M20] * val[M32] -
			val[M00] * val[M23] * val[M32] - val[M02] * val[M20] * val[M33] + val[M00] * val[M22] * val[M33];
	b.val[M12] = val[M03] * val[M12] * val[M30] - val[M02] * val[M13] * val[M30] - val[M03] * val[M10] * val[M32] +
			val[M00] * val[M13] * val[M32] + val[M02] * val[M10] * val[M33] - val[M00] * val[M12] * val[M33];
	b.val[M13] = val[M02] * val[M13] * val[M20] - val[M03] * val[M12] * val[M20] + val[M03] * val[M10] * val[M22] -
			val[M00] * val[M13] * val[M22] - val[M02] * val[M10] * val[M23] + val[M00] * val[M12] * val[M23];
	b.val[M20] = val[M11] * val[M23] * val[M30] - val[M13] * val[M21] * val[M30] + val[M13] * val[M20] * val[M31] -
			val[M10] * val[M23] * val[M31] - val[M11] * val[M20] * val[M33] + val[M10] * val[M21] * val[M33];
	b.val[M21] = val[M03] * val[M21] * val[M30] - val[M01] * val[M23] * val[M30] - val[M03] * val[M20] * val[M31] +
			val[M00] * val[M23] * val[M31] + val[M01] * val[M20] * val[M33] - val[M00] * val[M21] * val[M33];
	b.val[M22] = val[M01] * val[M13] * val[M30] - val[M03] * val[M11] * val[M30] + val[M03] * val[M10] * val[M31] -
			val[M00] * val[M13] * val[M31] - val[M01] * val[M10] * val[M33] + val[M00] * val[M11] * val[M33];
	b.val[M23] = val[M03] * val[M11] * val[M20] - val[M01] * val[M13] * val[M20] - val[M03] * val[M10] * val[M21] +
			val[M00] * val[M13] * val[M21] + val[M01] * val[M10] * val[M23] - val[M00] * val[M11] * val[M23];
	b.val[M30] = val[M12] * val[M21] * val[M30] - val[M11] * val[M22] * val[M30] - val[M12] * val[M20] * val[M31] +
			val[M10] * val[M22] * val[M31] + val[M11] * val[M20] * val[M32] - val[M10] * val[M21] * val[M32];
	b.val[M31] = val[M01] * val[M22] * val[M30] - val[M02] * val[M21] * val[M30] + val[M02] * val[M20] * val[M31] -
			val[M00] * val[M22] * val[M31] - val[M01] * val[M20] * val[M32] + val[M00] * val[M21] * val[M32];
	b.val[M32] = val[M02] * val[M11] * val[M30] - val[M01] * val[M12] * val[M30] - val[M02] * val[M10] * val[M31] +
			val[M00] * val[M12] * val[M31] + val[M01] * val[M10] * val[M32] - val[M00] * val[M11] * val[M32];
	b.val[M33] = val[M01] * val[M12] * val[M20] - val[M02] * val[M11] * val[M20] + val[M02] * val[M10] * val[M21] -
			val[M00] * val[M12] * val[M21] - val[M01] * val[M10] * val[M22] + val[M00] * val[M11] * val[M22];
	
	for(int i = 0; i < 16; i++)
		val[i] = b.val[i] / rdet;
	
	if(success != nullptr)
		*success = true;
	
	return *this;
}

Matrix4& Matrix4::tra()
{
	Matrix4 b;
	b.val[M00] = val[M00];
	b.val[M01] = val[M10];
	b.val[M02] = val[M20];
	b.val[M03] = val[M30];
	b.val[M10] = val[M01];
	b.val[M11] = val[M11];
	b.val[M12] = val[M21];
	b.val[M13] = val[M31];
	b.val[M20] = val[M02];
	b.val[M21] = val[M12];
	b.val[M22] = val[M22];
	b.val[M23] = val[M32];
	b.val[M30] = val[M03];
	b.val[M31] = val[M13];
	b.val[M32] = val[M23];
	b.val[M33] = val[M33];
	return *this = b;
}

Matrix4& Matrix4::rotate(const Vector3& vec, float rad)
{
	Quaternion quat(vec, rad);
	return *this *= quat;
}

float Matrix4::det()
{
	return val[M30] * val[M21] * val[M12] * val[M03] - val[M20] * val[M31] * val[M12] * val[M03] -
			val[M30] * val[M11] * val[M22] * val[M03] + val[M10] * val[M31] * val[M22] * val[M03] +
			val[M20] * val[M11] * val[M32] * val[M03] - val[M10] * val[M21] * val[M32] * val[M03] -
			val[M30] * val[M21] * val[M02] * val[M13] + val[M20] * val[M31] * val[M02] * val[M13] +
			val[M30] * val[M01] * val[M22] * val[M13] - val[M00] * val[M31] * val[M22] * val[M13] -
			val[M20] * val[M01] * val[M32] * val[M13] + val[M00] * val[M21] * val[M32] * val[M13] +
			val[M30] * val[M11] * val[M02] * val[M23] - val[M10] * val[M31] * val[M02] * val[M23] -
			val[M30] * val[M01] * val[M12] * val[M23] + val[M00] * val[M31] * val[M12] * val[M23] +
			val[M10] * val[M01] * val[M32] * val[M23] - val[M00] * val[M11] * val[M32] * val[M23] -
			val[M20] * val[M11] * val[M02] * val[M33] + val[M10] * val[M21] * val[M02] * val[M33] +
			val[M20] * val[M01] * val[M12] * val[M33] - val[M00] * val[M21] * val[M12] * val[M33] -
			val[M10] * val[M01] * val[M22] * val[M33] + val[M00] * val[M11] * val[M22] * val[M33];
}
