#include "FEMath.h"

FEVector2 FEVector2::operator+(const FEVector2& rhs)
{
	FEVector2 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	return v;
}
FEVector2 FEVector2::operator-(const FEVector2& rhs)
{
	FEVector2 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	return v;
}
FEVector2 FEVector2::operator+(const float& rhs)
{
	FEVector2 v;
	v.x = x + rhs;
	v.y = y + rhs;
	return v;
}
FEVector2 FEVector2::operator-(const float& rhs)
{
	FEVector2 v;
	v.x = x - rhs;
	v.y = y - rhs;
	return v;
}
FEVector2 FEVector2::operator*(const float& rhs)
{
	FEVector2 v;
	v.x = x * rhs;
	v.y = y * rhs;
	return v;
}
FEVector2 FEVector2::operator/(const float& rhs)
{
	FEVector2 v;
	v.x = x / rhs;
	v.y = y / rhs;
	return v;
}
FEVector2 operator+(const float lhs, const FEVector2& rhs)
{
	FEVector2 v;
	v.x = lhs + rhs.x;
	v.y = lhs + rhs.y;
	return v;
}
FEVector2 operator-(const float lhs, const FEVector2& rhs)
{
	FEVector2 v;
	v.x = lhs - rhs.x;
	v.y = lhs - rhs.y;
	return v;
}
FEVector2 operator*(const float lhs, const FEVector2& rhs)
{
	FEVector2 v;
	v.x = lhs * rhs.x;
	v.y = lhs * rhs.y;
	return v;
}


FEVector3 FEVector3::operator+(const FEVector3& rhs)
{
	FEVector3 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	v.z = z + rhs.z;
	return v;
}
FEVector3 FEVector3::operator-(const FEVector3& rhs)
{
	FEVector3 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	v.z = z - rhs.z;
	return v;
}
FEVector3 FEVector3::operator+(const float& rhs)
{
	FEVector3 v;
	v.x = x + rhs;
	v.y = y + rhs;
	v.z = z + rhs;
	return v;
}
FEVector3 FEVector3::operator-(const float& rhs)
{
	FEVector3 v;
	v.x = x - rhs;
	v.y = y - rhs;
	v.z = z - rhs;
	return v;
}
FEVector3 FEVector3::operator*(const float& rhs)
{
	FEVector3 v;
	v.x = x * rhs;
	v.y = y * rhs;
	v.z = z * rhs;
	return v;
}
FEVector3 FEVector3::operator/(const float& rhs)
{
	FEVector3 v;
	v.x = x / rhs;
	v.y = y / rhs;
	v.z = z / rhs;
	return v;
}
FEVector3 operator+(const float lhs, const FEVector3& rhs)
{
	FEVector3 v;
	v.x = lhs + rhs.x;
	v.y = lhs + rhs.y;
	v.z = lhs + rhs.z;
	return v;
}
FEVector3 operator-(const float lhs, const FEVector3& rhs)
{
	FEVector3 v;
	v.x = lhs - rhs.x;
	v.y = lhs - rhs.y;
	v.z = lhs - rhs.z;
	return v;
}
FEVector3 operator*(const float lhs, const FEVector3& rhs)
{
	FEVector3 v;
	v.x = lhs * rhs.x;
	v.y = lhs * rhs.y;
	v.z = lhs * rhs.z;
	return v;
}


FEVector4 FEVector4::operator+(const FEVector4& rhs)
{
	FEVector4 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	v.z = z + rhs.z;
	v.w = w + rhs.w;
	return v;
}
FEVector4 FEVector4::operator-(const FEVector4& rhs)
{
	FEVector4 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	v.z = z - rhs.z;
	v.w = w - rhs.w;
	return v;
}
FEVector4 FEVector4::operator+(const float& rhs)
{
	FEVector4 v;
	v.x = x + rhs;
	v.y = y + rhs;
	v.z = z + rhs;
	v.w = w + rhs;
	return v;
}
FEVector4 FEVector4::operator-(const float& rhs)
{
	FEVector4 v;
	v.x = x - rhs;
	v.y = y - rhs;
	v.z = z - rhs;
	v.w = w - rhs;
	return v;
}
FEVector4 FEVector4::operator*(const float& rhs)
{
	FEVector4 v;
	v.x = x * rhs;
	v.y = y * rhs;
	v.z = z * rhs;
	v.w = w * rhs;
	return v;
}
FEVector4 FEVector4::operator/(const float& rhs)
{
	FEVector4 v;
	v.x = x / rhs;
	v.y = y / rhs;
	v.z = z / rhs;
	v.w = w / rhs;
	return v;
}
FEVector4 operator+(const float lhs, const FEVector4& rhs)
{
	FEVector4 v;
	v.x = lhs + rhs.x;
	v.y = lhs + rhs.y;
	v.z = lhs + rhs.z;
	v.w = lhs + rhs.w;
	return v;
}
FEVector4 operator-(const float lhs, const FEVector4& rhs)
{
	FEVector4 v;
	v.x = lhs - rhs.x;
	v.y = lhs - rhs.y;
	v.z = lhs - rhs.z;
	v.w = lhs - rhs.w;
	return v;
}
FEVector4 operator*(const float lhs, const FEVector4& rhs)
{
	FEVector4 v;
	v.x = lhs * rhs.x;
	v.y = lhs * rhs.y;
	v.z = lhs * rhs.z;
	v.w = lhs * rhs.w;
	return v;
}