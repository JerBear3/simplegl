#ifndef MATH_HPP
#define MATH_HPP

#include <math.h>

#define M00 0
#define M01 4
#define M02 8
#define M03 12
#define M10 1
#define M11 5
#define M12 9
#define M13 13
#define M20 2
#define M21 6
#define M22 10
#define M23 14
#define M30 3
#define M31 7
#define M32 11
#define M33 15

#define V0 0.f, 0.f, 0.f
#define V1 1.f, 1.f, 1.f
#define VX 1.f, 0.f, 0.f
#define VY 0.f, 1.f, 0.f
#define VZ 0.f, 0.f, 1.f
#define VNX -1.f,  0.f,  0.f
#define VNY  0.f, -1.f,  0.f
#define VNZ  0.f,  0.f, -1.f

#define PIH M_PI / 2.f //90 degrees
#define PI M_PI //180 degrees
#define PI2 2.f * M_PI //360 degrees

#define RADDEG 180.f / PI
#define DEGRAD PI / 180.f


struct Matrix4;
struct Quaternion;
struct Vector3;
struct SimpleTransform;

struct Matrix4
{
	float val[16];
	
	Matrix4();
	Matrix4(const Quaternion& quat);
	Matrix4(const SimpleTransform& st);
	Matrix4(const Vector3& position, const Vector3& direction, const Vector3& up);
	Matrix4(float near, float far, float fovy, float aspectRatio);
	
	float& operator[](int i);
	
	Matrix4& operator=(const Quaternion& quat);
	Matrix4& operator=(const SimpleTransform& vec);
	
	Matrix4 operator*(const Matrix4& b) const;
	Matrix4 operator*(const Quaternion& quat) const;
	Matrix4 operator*(const Vector3& vec) const;
	Matrix4 operator*(const SimpleTransform& st) const;
	Matrix4& operator*=(const Matrix4& b);
	Matrix4& operator*=(const Quaternion& quat);
	Matrix4& operator*=(const Vector3& vec);
	Matrix4& operator*=(const SimpleTransform& st);
	
	Matrix4 operator+(const Vector3& vec) const;
	Matrix4 operator-(const Vector3& vec) const;
	Matrix4& operator+=(const Vector3& vec);
	Matrix4& operator-=(const Vector3& vec);
	
	Matrix4& setTranslation(const Vector3& vec);
	Matrix4& setScale(const Vector3& vec);
	Matrix4& setOrthographic(float left, float right, float bottom, float top, float near, float far);
	Matrix4& setPerspective(float near, float far, float fovy, float aspectRatio);
	Matrix4& setView(const Vector3& position, const Vector3& direction, const Vector3& up);
	
	Matrix4& calculateNormal(Matrix4& out) const; //make normal matrix from transform matrix
	
	Matrix4& idt();
	Matrix4& inv(bool* success = nullptr);
	Matrix4& tra();
	Matrix4& rotate(const Vector3& vec, float rad);
	
	float det();
};

struct Quaternion
{
	float x, y, z, w;
	
	Quaternion();
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Quaternion(const Vector3& axis, float rad);
	
	Quaternion& set(float x, float y, float z, float w);
	Quaternion& set(const Vector3& axis, float rad);
	Quaternion& idt();
	Quaternion& nor();
};

struct Vector3
{
	float x, y, z;
	
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	
	Vector3 operator*(float scalar) const;
	Vector3 operator*(const Matrix4& quat) const;
	Vector3 operator*(const Quaternion& quat) const;
	Vector3& operator*=(float scalar);
	Vector3& operator*=(const Matrix4& quat);
	Vector3& operator*=(const Quaternion& quat);
	
	Vector3 operator+(const Vector3& b) const;
	Vector3 operator-(const Vector3& b) const;
	Vector3& operator+=(const Vector3& b);
	Vector3& operator-=(const Vector3& b);
	
	Vector3& set(float x, float y, float z);
	Vector3& nor();
	Vector3& crs(const Vector3& b);
	Vector3& rotate(const Vector3& vec, float rad);
	
	float len2() const;
	float len() const;
	float dot(const Vector3& b) const;
};

struct SimpleTransform
{
	SimpleTransform() : scale(V1) {}
	SimpleTransform(const Vector3& translation, const Quaternion& rotation, const Vector3& scale) :
			translation(translation), rotation(rotation), scale(scale) {}
	
	Vector3 translation;
	Quaternion rotation;
	Vector3 scale;
};

#endif
