#include "FEObjectHeader.h"

using namespace FEMath;

FETransform::FETransform()
{
}


FETransform::~FETransform()
{
}


FEMatrix FETransform::GetWorldMatrix()
{
	FEMatrix mPos, mRot, mScale;
	FEMatrix mWorld;

	FEGameObject* pObj = GetMyObject();

	mPos = FEMatrixTranslation(_vPos);
	mRot = FEMatrixRotationRollPitchYaw(_vRot);
	mScale = FEMatrixScaling(m_vScale);

	if (pObj->GetParent() != nullptr)
		mScale *= pObj->GetParent()->GetTransform()->GetScaleMatrix();

	mWorld = mScale * mRot * mPos;

	if (pObj->GetParent() != nullptr)
		mWorld *= pObj->GetParent()->GetTransform()->GetRotPosMatrix();

	return mWorld;
}


FEMatrix FETransform::GetPositionMatrix()
{
	FEGameObject* pObj = GetMyObject();

	FEMatrix mPos;
	mPos = FEMatrixTranslation(_vPos);

	if (pObj->GetParent() != nullptr)
		mPos *= pObj->GetParent()->GetTransform()->GetPositionMatrix();

	return mPos;
}


FEMatrix FETransform::GetRotationMatrix()
{
	FEGameObject* pObj = GetMyObject();

	FEMatrix mRot;
	mRot = FEMatrixRotationRollPitchYaw(_vRot);

	if (pObj->GetParent() != nullptr)
		mRot *= pObj->GetParent()->GetTransform()->GetRotationMatrix();

	return mRot;
}


FEMatrix FETransform::GetRotPosMatrix()
{
	FEGameObject* pObj = GetMyObject();

	FEMatrix mPos, mRot, mRP;
	mPos = FEMatrixTranslation(_vPos);
	mRot = FEMatrixRotationRollPitchYaw(_vRot);

	mRP = mRot * mPos;

	if (pObj->GetParent() != nullptr)
		mRP *= pObj->GetParent()->GetTransform()->GetRotPosMatrix();

	return mRP;
}


FEMatrix FETransform::GetScaleMatrix()
{
	FEGameObject* pObj = GetMyObject();

	FEMatrix mScale;
	mScale = FEMatrixScaling(m_vScale);

	if (pObj->GetParent() != nullptr)
		mScale *= pObj->GetParent()->GetTransform()->GetScaleMatrix();

	return mScale;
}


void FETransform::Translate(const FEVector3& translate, Space space)
{
	if (space)
		_vPos += translate;
	else
		SetPositionWorld(GetPositionWorld() + translate);
}

void FETransform::Rotate(const FEVector3& eulerAngles, Space space)
{
	if (space)
		_vRot += eulerAngles / 180.0f * FE_PI;
	else
		_vRot += eulerAngles / 180.0f * FE_PI;
}


void FETransform::SetPositionWorld(const FEVector3& pos)
{
	FEVector3 v = pos - GetMyObject()->GetParent()->GetTransform()->GetPositionWorld();

	_vPos = FEVector3(v * GetRotationMatrix().Inverse());
}


void FETransform::SetPositionLocal(const FEVector3& pos)
{
	_vPos = pos;
}


void FETransform::SetRotationDegree(const FEVector3& angle)
{
	_vRot = angle * (FE_PI / 180.0f);
}


void FETransform::SetRotationRadian(const FEVector3& radian)
{
	_vRot = radian;
}


FEVector3 FETransform::GetPositionWorld()
{
	return Vector3::Zero * GetRotPosMatrix();
}


FEVector3 FETransform::GetPositionLocal()
{
	return _vPos;
}


FEVector3 FETransform::GetRotationRadian()
{
	return _vRot;
}


FEVector3 FETransform::GetRotationDegree()
{
	return _vRot * (FE_PI / 180.0f);
}


FEVector3 FETransform::GetLookAt()
{
	return FEVector3::Forward * FEMatrixRotationRollPitchYaw(_vRot);
}


FEVector3 FETransform::GetUpVector()
{
	return FEVector3::Up * FEMatrixRotationRollPitchYaw(_vRot);
}


FEVector3 FETransform::GetRightVector()
{
	return FEVector3::Right * FEMatrixRotationRollPitchYaw(_vRot);
}