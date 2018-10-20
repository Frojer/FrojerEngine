#pragma once
#ifndef _FE_TRANSFORM
#define _FE_TRANSFORM

#include <FEMath.h>
#include "FEObjectHeader.h"

class FEComponent;

enum Space { World, Local };

class FETransform :	public FEComponent
{
private:
	FEVector3 _vPos;
	FEVector4 _qRot;
	FEVector4 _qScaleRot;

public:
	FEVector3 m_vScale;

public:
	FETransform();
	virtual ~FETransform();

	void Translate(const FEVector3& translation, Space space = Space::Local);
	void Rotate(const FEVector3& eulerAngles, Space space = Space::Local);
	void Rotate(const FEVector3& axis, float angle);

	// Get(), Set()
	void SetPositionWorld(const FEVector3& pos);
	void SetPositionLocal(const FEVector3& pos);
	void SetRotationQuaternion(const FEVector4& Q);
	void SetRotationDegree(const FEVector3& degree);
	void SetRotationRadian(const FEVector3& radian);
	FEVector3 GetPositionWorld();
	FEVector3 GetPositionLocal();
	FEVector4 GetRotationQuaternion();

	FEVector3 GetLookAt();
	FEVector3 GetUpVector();
	FEVector3 GetRightVector();


	FEMatrix GetWorldMatrix();
	FEMatrix GetPositionMatrix();
	FEMatrix GetRotationMatrix();
	FEMatrix GetRotPosMatrix();
	FEMatrix GetScaleMatrix();
};

#endif