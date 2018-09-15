#pragma once
#ifndef _FE_MATH
#define _FE_MATH

#define FE_PI 3.141592654f
#define FE_2PI 6.283185307f
#define FE_1DIVPI 0.318309886f
#define FE_1DIV2PI 0.159154943f
#define FE_PIDIV2 1.570796327f
#define FE_PIDIV4 0.785398163f

#ifdef _WIN32
#include <DirectXMath.h>

class FEVector2;
class FEVector3;
class FEVector4;
class FEMatrix;

typedef DirectX::XMVECTOR FEVectorA;
typedef DirectX::XMMATRIX FEMatrixA;

namespace FEMath
{
	typedef FEVector2 Vector2;
	typedef FEVector3 Vector3;
	typedef FEVector4 Vector4;
	typedef FEMatrix Matrix;

	FEMatrix FEMatrixTranslation(const FEVector3& Offset);
	FEMatrix FEMatrixRotationRollPitchYaw(const FEVector3& Angles);
	FEMatrix FEMatrixScaling(const FEVector3& Scale);

	FEVectorA FEConvertToAlignData(const FEVector2& V);
	FEVectorA FEConvertToAlignData(const FEVector3& V);
	FEVectorA FEConvertToAlignData(const FEVector4& V);
	FEMatrixA FEConvertToAlignData(const FEMatrix& M);
}

class FEVector2 : public DirectX::XMFLOAT2
{
public:
	static const FEVector2 Zero;
	static const FEVector2 One;
	static const FEVector2 Left;
	static const FEVector2 Right;
	static const FEVector2 Up;
	static const FEVector2 Down;

public:
	explicit FEVector2(float x = 0.0f, float y = 0.0f) : XMFLOAT2(x, y) {};
	FEVector2(const FEVector2& v) : XMFLOAT2(v) {};
	FEVector2(const FEVector3& v);
	FEVector2(const FEVector4& v);
	FEVector2(const FEVectorA& v);
	~FEVector2() = default;

	//----------------
	// 연산자 오버로딩
	//----------------
	bool operator == (const FEVector2& rhs) const;
	bool operator != (const FEVector2& rhs) const;

	FEVector2& operator= (const FEVectorA& rhs);
	FEVector2& operator+= (const FEVector2& rhs);
	FEVector2& operator-= (const FEVector2& rhs);
	FEVector2& operator*= (const FEVector2& rhs);
	FEVector2& operator*= (float rhs);
	FEVector2& operator/= (float rhs);
	FEVector2 operator+ () const;
	FEVector2 operator- () const;
	FEVector2 operator+(const FEVector2& rhs) const;
	FEVector2 operator-(const FEVector2& rhs) const;
	FEVector2 operator+(const float& rhs) const;
	FEVector2 operator-(const float& rhs) const;
	FEVector2 operator*(const float& rhs) const;
	FEVector2 operator/(const float& rhs) const;
};
FEVector2 operator+(const float lhs, const FEVector2& rhs);
FEVector2 operator-(const float lhs, const FEVector2& rhs);
FEVector2 operator*(const float lhs, const FEVector2& rhs);

class FEVector3 : public DirectX::XMFLOAT3
{
public:
	static const FEVector3 Zero;
	static const FEVector3 One;
	static const FEVector3 Left;
	static const FEVector3 Right;
	static const FEVector3 Up;
	static const FEVector3 Down;
	static const FEVector3 Forward;
	static const FEVector3 Back;

public:
	explicit FEVector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : XMFLOAT3(x, y, z) {};
	FEVector3(const FEVector3& v) : XMFLOAT3(v) {};
	FEVector3(const FEVector2& v, float z = 0.0f) : XMFLOAT3(v.x, v.y, z) {};
	FEVector3(const FEVector4& v);
	FEVector3(const FEVectorA& v);
	~FEVector3() = default;

	//----------------
	// 연산자 오버로딩
	//----------------
	bool operator ==(const FEVector3& rhs) const;
	bool operator !=(const FEVector3& rhs) const;
	FEVector3& operator= (const FEVectorA& rhs);
	FEVector3& operator+=(const FEVector3& rhs);
	FEVector3& operator-=(const FEVector3& rhs);
	FEVector3& operator*=(const FEVector3& rhs);
	FEVector3& operator*=(float rhs);
	FEVector3& operator/=(float rhs);
	FEVector3 operator+() const;
	FEVector3 operator-() const;
	FEVector3 operator+(const FEVector3& rhs) const;
	FEVector3 operator-(const FEVector3& rhs) const;
	FEVector3 operator+(const float& rhs) const;
	FEVector3 operator-(const float& rhs) const;
	FEVector3 operator*(const float& rhs) const;
	FEVector3 operator/(const float& rhs) const;
};
FEVector3 operator+(const float lhs, const FEVector3& rhs);
FEVector3 operator-(const float lhs, const FEVector3& rhs);
FEVector3 operator*(const float lhs, const FEVector3& rhs);

class FEVector4 : public DirectX::XMFLOAT4
{
public:
	static const FEVector4 Zero;
	static const FEVector4 One;

public:
	explicit FEVector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : XMFLOAT4(x, y, z, w) {};
	FEVector4(const FEVector4& v) : XMFLOAT4(v) {};
	FEVector4(const FEVector2& xy, const FEVector2& zw) : XMFLOAT4(xy.x, xy.y, zw.x, zw.y) {};
	FEVector4(const FEVector2& v, float z = 0.0f, float w = 0.0f) : XMFLOAT4(v.x, v.y, z, w) {};
	FEVector4(const FEVector3& v, float w = 0.0f) : XMFLOAT4(v.x, v.y, v.z, w) {};
	FEVector4(const FEVectorA& v);
	~FEVector4() = default;

	//----------------
	// 연산자 오버로딩
	//----------------
	bool operator ==(const FEVector4& rhs) const;
	bool operator !=(const FEVector4& rhs) const;
	FEVector4& operator= (const FEVectorA& rhs);
	FEVector4& operator+=(const FEVector4& rhs);
	FEVector4& operator-=(const FEVector4& rhs);
	FEVector4& operator*=(const FEVector4& rhs);
	FEVector4& operator*=(float rhs);
	FEVector4& operator/=(float rhs);
	FEVector4 operator+() const;
	FEVector4 operator-() const;
	FEVector4 operator+(const FEVector4& rhs) const;
	FEVector4 operator-(const FEVector4& rhs) const;
	FEVector4 operator+(const float& rhs) const;
	FEVector4 operator-(const float& rhs) const;
	FEVector4 operator*(const float& rhs) const;
	FEVector4 operator/(const float& rhs) const;

	FEVector4& operator*= (const FEMatrix& rhs);
};
FEVector4 operator+(const float lhs, const FEVector4& rhs);
FEVector4 operator-(const float lhs, const FEVector4& rhs);
FEVector4 operator*(const float lhs, const FEVector4& rhs);

class FEMatrix : public DirectX::XMFLOAT4X4
{
public:
	FEMatrix() = default;
	FEMatrix(const FEVector4& v1, const FEVector4& v2, const FEVector4& v3, const FEVector4& v4)
		: XMFLOAT4X4(v1.x, v1.y, v1.z, v1.w,
					v2.x, v2.y, v2.z, v2.w,
					v3.x, v3.y, v3.z, v3.w,
					v4.x, v4.y, v4.z, v4.w) {};
	FEMatrix(const FEMatrixA& m);
	~FEMatrix() = default;

	//----------
	// 맴버 함수
	//----------
	void SetIdentity();
	FEMatrix& Inverse(FEVector4* pDeterminant = nullptr);

	//----------------
	// 연산자 오버로딩
	//----------------
	FEMatrix& operator= (const FEMatrixA& rhs);
	FEMatrix& operator*= (const FEMatrix& rhs);
	FEMatrix operator*(const FEMatrix& rhs) const;
	FEVector4 operator*(const FEVector4& rhs) const;
};
FEVector3 operator*(const FEVector3& lhs, const FEMatrix& rhs);
FEVector4 operator*(const FEVector4& lhs, const FEMatrix& rhs);

#elif
// GL Math
#endif

FEMatrix FEMatrixLookAtLH(FEVector4 EyePosition, FEVector4 FocusPosition, FEVector4 UpDirection);
FEMatrix FEMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
#endif