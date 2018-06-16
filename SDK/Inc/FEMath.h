#pragma once
#ifndef _FE_MATH
#define _FE_MATH

class FEVector2
{
public:
	float x, y;
public:
	explicit FEVector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {};
	FEVector2(const FEVector2& v) : x(v.x), y(v.y) {};
	~FEVector2() = default;

	FEVector2 operator+(const FEVector2& rhs);
	FEVector2 operator-(const FEVector2& rhs);
	FEVector2 operator+(const float& rhs);
	FEVector2 operator-(const float& rhs);
	FEVector2 operator*(const float& rhs);
	FEVector2 operator/(const float& rhs);
};
FEVector2 operator+(const float lhs, const FEVector2& rhs);
FEVector2 operator-(const float lhs, const FEVector2& rhs);
FEVector2 operator*(const float lhs, const FEVector2& rhs);

class FEVector3
{
public:
	float x, y, z;
public:
	explicit FEVector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {};
	FEVector3(const FEVector3& v) : x(v.x), y(v.y), z(v.z) {};
	FEVector3(const FEVector2& v, float z = 0.0f) : x(v.x), y(v.y), z(z) {};
	~FEVector3() = default;

	FEVector3 operator+(const FEVector3& rhs);
	FEVector3 operator-(const FEVector3& rhs);
	FEVector3 operator+(const float& rhs);
	FEVector3 operator-(const float& rhs);
	FEVector3 operator*(const float& rhs);
	FEVector3 operator/(const float& rhs);
};
FEVector3 operator+(const FEVector3& lhs, const float rhs);
FEVector3 operator-(const FEVector3& lhs, const float rhs);
FEVector3 operator*(const FEVector3& lhs, const float rhs);
FEVector3 operator/(const FEVector3& lhs, const float rhs);

class FEVector4
{
public:
	float x, y, z, w;
public:
	explicit FEVector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {};
	FEVector4(const FEVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
	FEVector4(const FEVector2& xy, const FEVector2& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {};
	FEVector4(const FEVector2& v, float z = 0.0f, float w = 0.0f) : x(v.x), y(v.y), z(z), w(w) {};
	FEVector4(const FEVector3& v, float w = 0.0f) : x(v.x), y(v.y), z(v.z), w(w) {};
	~FEVector4() = default;

	FEVector4 operator+(const FEVector4& rhs);
	FEVector4 operator-(const FEVector4& rhs);
	FEVector4 operator+(const float& rhs);
	FEVector4 operator-(const float& rhs);
	FEVector4 operator*(const float& rhs);
	FEVector4 operator/(const float& rhs);
};
FEVector4 operator+(const FEVector4& lhs, const float rhs);
FEVector4 operator-(const FEVector4& lhs, const float rhs);
FEVector4 operator*(const FEVector4& lhs, const float rhs);
FEVector4 operator/(const FEVector4& lhs, const float rhs);

#endif