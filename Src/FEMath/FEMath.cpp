#include "FEMath.h"

float FEConvertToDegree(float radian)
{
	return radian / FE_PI * 180;
}
float FEConvertToRadian(float degree)
{
	return degree / 180 * FE_PI;
}

using namespace DirectX;

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
const FEMatrix FEMatrix::Identity	(XMMatrixIdentity());


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
FEVector2 FEMath::FEConvertToVector2(const FEVectorA& V)
{
	FEVector2 ret;
	XMStoreFloat2(&ret, V);
	return ret;
}
FEVector3 FEMath::FEConvertToVector3(const FEVectorA& V)
{
	FEVector3 ret;
	XMStoreFloat3(&ret, V);
	return ret;
}
FEVector4 FEMath::FEConvertToVector4(const FEVectorA& V)
{
	FEVector4 ret;
	XMStoreFloat4(&ret, V);
	return ret;
}
FEMatrix FEMath::FEConvertToMatrix(const FEMatrixA& M)
{
	FEMatrix ret;
	XMStoreFloat4x4(&ret, M);
	return ret;
}
FEVectorA FEMath::operator*(const FEVectorA& V, const FEMatrixA& M)
{
	return XMVector4Transform(V, M);
}


FEVector2::FEVector2(const FEVector3& v) : XMFLOAT2(v.x, v.y) {};
FEVector2::FEVector2(const FEVector4& v) : XMFLOAT2(v.x, v.y) {};
FEVector2::FEVector2(const FEVectorA& v)
{
	*this = FEMath::FEConvertToVector2(v);
}
FEVector3::FEVector3(const FEVector4& v) : XMFLOAT3(v.x, v.y, v.z) {};
FEVector3::FEVector3(const FEVectorA& v)
{
	*this = FEMath::FEConvertToVector3(v);
}
FEVector4::FEVector4(const FEVectorA& v)
{
	*this = FEMath::FEConvertToVector4(v);
}
FEMatrix::FEMatrix(const FEMatrixA& m)
{
	*this = FEMath::FEConvertToMatrix(m);
}

bool FEVector2::operator==(const FEVector2& rhs) const
{
	return x == rhs.x && y == rhs.y ? true : false;
}
bool FEVector2::operator!=(const FEVector2& rhs) const
{
	return x != rhs.x && y != rhs.y ? true : false;
}
FEVector2& FEVector2::operator= (const FEVectorA& rhs)
{
	*this = FEMath::FEConvertToVector2(rhs);

	return *this;
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
FEVector3& FEVector3::operator= (const FEVectorA& rhs)
{
	*this = FEMath::FEConvertToVector3(rhs);

	return *this;
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
FEVector4& FEVector4::operator= (const FEVectorA& rhs)
{
	*this = FEMath::FEConvertToVector4(rhs);

	return *this;
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
	FEVectorA v = FEMath::FEConvertToAlignData(*this);
	FEMatrixA m = FEMath::FEConvertToAlignData(rhs);

	*this = FEMath::FEConvertToVector4(XMVector4Transform(v, m));

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
FEVector4 FEVector4::operator*(const FEMatrix& rhs) const
{
	FEVector4 ret;

	FEVectorA v = FEMath::FEConvertToAlignData(*this);
	FEMatrixA m = FEMath::FEConvertToAlignData(rhs);

	ret = FEMath::FEConvertToVector4(XMVector4Transform(v, m));

	return ret;
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

FEMatrix FEMatrix::Inverse(FEMatrix m, FEVector4* pDeterminant)
{
	return FEMath::FEConvertToMatrix(XMMatrixInverse(pDeterminant == nullptr ? nullptr : &FEMath::FEConvertToAlignData(*pDeterminant), FEMath::FEConvertToAlignData(m)));
}

void FEMatrix::SetIdentity()
{
	*this = FEMatrix::Identity;
}
FEMatrix& FEMatrix::Inverse(FEVector4* pDeterminant)
{
	*this = FEMath::FEConvertToMatrix(XMMatrixInverse(pDeterminant == nullptr ? nullptr : &FEMath::FEConvertToAlignData(*pDeterminant), FEMath::FEConvertToAlignData(*this)));
	return *this;
}
FEMatrix& FEMatrix::operator= (const FEMatrixA& rhs)
{
	*this = FEMath::FEConvertToMatrix(rhs);
	return *this;
}
FEMatrix& FEMatrix::operator*= (const FEMatrix& rhs)
{
	FEMatrixA m1 = FEMath::FEConvertToAlignData(*this);
	FEMatrixA m2 = FEMath::FEConvertToAlignData(rhs);

	*this = FEMath::FEConvertToMatrix(m1 * m2);
	return *this;
}
FEMatrix FEMatrix::operator*(const FEMatrix& rhs) const
{
	FEMatrix result;
	FEMatrixA m1 = FEMath::FEConvertToAlignData(*this);
	FEMatrixA m2 = FEMath::FEConvertToAlignData(rhs);

	result = FEMath::FEConvertToMatrix(m1 * m2);
	return result;
}
FEVector3 operator*(const FEVector3& lhs, const FEMatrix& rhs)
{
	FEVector3 result;
	FEVectorA v = FEMath::FEConvertToAlignData(lhs);
	FEMatrixA m = FEMath::FEConvertToAlignData(rhs);

	result = FEMath::FEConvertToVector3(XMVector3TransformCoord(v, m));
	return result;
}
FEVector4 operator*(const FEVector4& lhs, const FEMatrix& rhs)
{
	FEVector4 result;
	FEVectorA v = FEMath::FEConvertToAlignData(lhs);
	FEMatrixA m = FEMath::FEConvertToAlignData(rhs);

	result = FEMath::FEConvertToVector4(XMVector4Transform(v, m));
	return result;
}

FEMatrix FEMatrixLookAtLH(FEVector4 EyePosition, FEVector4 FocusPosition, FEVector4 UpDirection)
{
	return XMMatrixLookAtLH(XMLoadFloat4(&EyePosition), XMLoadFloat4(&FocusPosition), XMLoadFloat4(&UpDirection));
}
FEMatrix FEMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
	return XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
}