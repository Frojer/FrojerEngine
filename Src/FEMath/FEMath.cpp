#include "FEMath.h"

const FEVector2 FEVector2::Zero		( 0.0f,  0.0f);
const FEVector2 FEVector2::One		( 1.0f,  1.0f);
const FEVector2 FEVector2::Left		( 1.0f,  0.0f);
const FEVector2 FEVector2::Right	(-1.0f,  0.0f);
const FEVector2 FEVector2::Up		( 0.0f,  1.0f);
const FEVector2 FEVector2::Down		( 0.0f, -1.0f);
const FEVector3 FEVector3::Zero		( 0.0f,  0.0f,  0.0f);
const FEVector3 FEVector3::One		( 1.0f,  1.0f,  1.0f);
const FEVector3 FEVector3::Left		( 1.0f,  0.0f,  0.0f);
const FEVector3 FEVector3::Right	(-1.0f,  0.0f,  0.0f);
const FEVector3 FEVector3::Up		( 0.0f,  1.0f,  0.0f);
const FEVector3 FEVector3::Down		( 0.0f,  -1.0f, 0.0f);
const FEVector3 FEVector3::Forward	( 0.0f,  0.0f,  1.0f);
const FEVector3 FEVector3::Back		( 0.0f,  0.0f, -1.0f);
const FEVector4 FEVector4::Zero		( 0.0f,  0.0f,  0.0f, 0.0f);
const FEVector4 FEVector4::One		( 1.0f,  1.0f,  1.0f, 1.0f);



using namespace DirectX;

FEMatrix FEMath::FEMatrixTranslation(const FEVector3& Offset)
{
	FEMatrix result;
	XMStoreFloat4x4(&result, XMMatrixTranslationFromVector(XMLoadFloat3(&Offset)));
	return result;
}
FEMatrix FEMath::FEMatrixRotationRollPitchYaw(const FEVector3& Angles)
{
	FEMatrix result;
	XMStoreFloat4x4(&result, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Angles)));
	return result;
}
FEMatrix FEMath::FEMatrixScaling(const FEVector3& Scale)
{
	FEMatrix result;
	XMStoreFloat4x4(&result, XMMatrixScalingFromVector(XMLoadFloat3(&Scale)));
	return result;
}


FEVectorA FEMath::FEConvertToAlignData(const FEVector2& V)
{
	return XMLoadFloat2(&V);
}
FEVectorA FEMath::FEConvertToAlignData(const FEVector3& V)
{
	return XMLoadFloat3(&V);
}
FEVectorA FEMath::FEConvertToAlignData(const FEVector4& V)
{
	return XMLoadFloat4(&V);
}
FEMatrixA FEMath::FEConvertToAlignData(const FEMatrix& M)
{
	return XMLoadFloat4x4(&M);
}


FEVector2::FEVector2(const FEVector3& v) : XMFLOAT2(v.x, v.y) {};
FEVector2::FEVector2(const FEVector4& v) : XMFLOAT2(v.x, v.y) {};
FEVector3::FEVector3(const FEVector4& v) : XMFLOAT3(v.x, v.y, v.z) {};

bool FEVector2::operator==(const FEVector2& rhs) const
{
	return x == rhs.x && y == rhs.y ? true : false;
}
bool FEVector2::operator!=(const FEVector2& rhs) const
{
	return x != rhs.x && y != rhs.y ? true : false;
}
FEVector2& FEVector2::operator+=(const FEVector2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}
FEVector2& FEVector2::operator-=(const FEVector2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
FEVector2& FEVector2::operator*=(const FEVector2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}
FEVector2& FEVector2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}
FEVector2& FEVector2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	return *this;
}
FEVector2 FEVector2::operator+() const
{
	return *this;
}
FEVector2 FEVector2::operator-() const
{
	return FEVector2(-x, -y);
}
FEVector2 FEVector2::operator+(const FEVector2& rhs) const
{
	FEVector2 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	return v;
}
FEVector2 FEVector2::operator-(const FEVector2& rhs) const
{
	FEVector2 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	return v;
}
FEVector2 FEVector2::operator+(const float& rhs) const
{
	FEVector2 v;
	v.x = x + rhs;
	v.y = y + rhs;
	return v;
}
FEVector2 FEVector2::operator-(const float& rhs) const
{
	FEVector2 v;
	v.x = x - rhs;
	v.y = y - rhs;
	return v;
}
FEVector2 FEVector2::operator*(const float& rhs) const
{
	FEVector2 v;
	v.x = x * rhs;
	v.y = y * rhs;
	return v;
}
FEVector2 FEVector2::operator/(const float& rhs) const
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



bool FEVector3::operator==(const FEVector3& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z ? true : false;
}
bool FEVector3::operator!=(const FEVector3& rhs) const
{
	return x != rhs.x && y != rhs.y && z == rhs.z ? true : false;
}
FEVector3& FEVector3::operator+=(const FEVector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}
FEVector3& FEVector3::operator-=(const FEVector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}
FEVector3& FEVector3::operator*=(const FEVector3& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}
FEVector3& FEVector3::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}
FEVector3& FEVector3::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}
FEVector3 FEVector3::operator+() const
{
	return *this;
}
FEVector3 FEVector3::operator-() const
{
	return FEVector3(-x, -y, -z);
}
FEVector3 FEVector3::operator+(const FEVector3& rhs) const
{
	FEVector3 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	v.z = z + rhs.z;
	return v;
}
FEVector3 FEVector3::operator-(const FEVector3& rhs) const
{
	FEVector3 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	v.z = z - rhs.z;
	return v;
}
FEVector3 FEVector3::operator+(const float& rhs) const
{
	FEVector3 v;
	v.x = x + rhs;
	v.y = y + rhs;
	v.z = z + rhs;
	return v;
}
FEVector3 FEVector3::operator-(const float& rhs) const
{
	FEVector3 v;
	v.x = x - rhs;
	v.y = y - rhs;
	v.z = z - rhs;
	return v;
}
FEVector3 FEVector3::operator*(const float& rhs) const
{
	FEVector3 v;
	v.x = x * rhs;
	v.y = y * rhs;
	v.z = z * rhs;
	return v;
}
FEVector3 FEVector3::operator/(const float& rhs) const
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



bool FEVector4::operator==(const FEVector4& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w ? true : false;
}
bool FEVector4::operator!=(const FEVector4& rhs) const
{
	return x != rhs.x && y != rhs.y && z == rhs.z && w == rhs.w ? true : false;
}
FEVector4& FEVector4::operator+=(const FEVector4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}
FEVector4& FEVector4::operator-=(const FEVector4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}
FEVector4& FEVector4::operator*=(const FEVector4& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}
FEVector4& FEVector4::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}
FEVector4& FEVector4::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;
	return *this;
}
FEVector4& FEVector4::operator*= (const FEMatrix& rhs)
{
	XMVECTOR v = XMLoadFloat4(this);
	XMMATRIX m = XMLoadFloat4x4(&rhs);

	XMStoreFloat4(this, XMVector4Transform(v, m));

	return *this;
}
FEVector4 FEVector4::operator+() const
{
	return *this;
}
FEVector4 FEVector4::operator-() const
{
	return FEVector4(-x, -y, -z, -w);
}
FEVector4 FEVector4::operator+(const FEVector4& rhs) const
{
	FEVector4 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;
	v.z = z + rhs.z;
	v.w = w + rhs.w;
	return v;
}
FEVector4 FEVector4::operator-(const FEVector4& rhs) const
{
	FEVector4 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;
	v.z = z - rhs.z;
	v.w = w - rhs.w;
	return v;
}
FEVector4 FEVector4::operator+(const float& rhs) const
{
	FEVector4 v;
	v.x = x + rhs;
	v.y = y + rhs;
	v.z = z + rhs;
	v.w = w + rhs;
	return v;
}
FEVector4 FEVector4::operator-(const float& rhs) const
{
	FEVector4 v;
	v.x = x - rhs;
	v.y = y - rhs;
	v.z = z - rhs;
	v.w = w - rhs;
	return v;
}
FEVector4 FEVector4::operator*(const float& rhs) const
{
	FEVector4 v;
	v.x = x * rhs;
	v.y = y * rhs;
	v.z = z * rhs;
	v.w = w * rhs;
	return v;
}
FEVector4 FEVector4::operator/(const float& rhs) const
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


void FEMatrix::SetIdentity()
{
	XMStoreFloat4x4(this, XMMatrixIdentity());
}
FEMatrix& FEMatrix::Inverse(FEVector4* pDeterminant)
{
	XMStoreFloat4x4(this, XMMatrixInverse(nullptr, XMLoadFloat4x4(this)));

	return *this;
}
FEMatrix& FEMatrix::operator*= (const FEMatrix& rhs)
{
	XMMATRIX m1 = XMLoadFloat4x4(this);
	XMMATRIX m2 = XMLoadFloat4x4(&rhs);

	XMStoreFloat4x4(this, XMMatrixMultiply(m1, m2));

	return *this;
}
FEMatrix FEMatrix::operator*(const FEMatrix& rhs) const
{
	FEMatrix result;
	XMMATRIX m1 = XMLoadFloat4x4(this);
	XMMATRIX m2 = XMLoadFloat4x4(&rhs);

	XMStoreFloat4x4(&result, XMMatrixMultiply(m1, m2));
	return result;
}
FEVector4 FEMatrix::operator*(const FEVector4& rhs) const
{
	FEVector4 result;
	XMMATRIX m = XMLoadFloat4x4(this);
	XMVECTOR v = XMLoadFloat4(&rhs);

	XMStoreFloat4(&result, XMVector4Transform(v, m));
	return result;
}
FEVector4 operator*(const FEVector4& lhs, const FEMatrix& rhs)
{
	FEVector4 result;
	XMVECTOR v = XMLoadFloat4(&lhs);
	XMMATRIX m = XMLoadFloat4x4(&rhs);

	XMStoreFloat4(&result, XMVector4Transform(v, m));
	return result;
}