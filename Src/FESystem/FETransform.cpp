#include "FEObjectHeader.h"

using namespace FEMath;

FETransform::FETransform()
	: _vPos(FEVector3::Zero), _qRot(0.0f, 0.0f, 0.0f, 1.0f), m_vScale(FEVector3::One)
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
	mRot = FEMatrixRotationQuaternion(_qRot);
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
	mRot = FEMatrixRotationQuaternion(_qRot);

	if (pObj->GetParent() != nullptr)
		mRot *= pObj->GetParent()->GetTransform()->GetRotationMatrix();

	return mRot;
}


FEMatrix FETransform::GetRotPosMatrix()
{
	FEGameObject* pObj = GetMyObject();

	FEMatrix mPos, mRot, mRP;
	mPos = FEMatrixTranslation(_vPos);
	mRot = FEMatrixRotationQuaternion(_qRot);

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
	//FEVector3 angle = eulerAngles / 180.0f * FE_PI;
	//_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationRollPitchYaw(angle));
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(GetRightVector(), FEConvertToRadian(eulerAngles.x)));
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(FEVector3::Up, FEConvertToRadian(eulerAngles.y)));
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(GetLookAt(), FEConvertToRadian(eulerAngles.z)));
}
void FETransform::Rotate(const FEVector3& axis, float angle)
{
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(axis, angle));
}


void FETransform::SetPositionWorld(const FEVector3& pos)
{
	FEVector3 v = pos;

	if (GetMyObject()->GetParent() != nullptr)
	v = pos - GetMyObject()->GetParent()->GetTransform()->GetPositionWorld();

	_vPos = FEVector3(v * GetRotationMatrix().Inverse());
}


void FETransform::SetPositionLocal(const FEVector3& pos)
{
	_vPos = pos;
}
void FETransform::SetRotationQuaternion(const FEVector4& Q)
{
	_qRot = Q;
}
void FETransform::SetRotationDegree(const FEVector3& angle)
{
	_qRot = FEQuaternionRotationAxis(GetRightVector(), FEConvertToRadian(angle.x));
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(FEVector3::Up, FEConvertToRadian(angle.y)));
	_qRot = FEQuaternionMultiply(_qRot, FEQuaternionRotationAxis(FEVector3::Forward, FEConvertToRadian(angle.z)));
}


void FETransform::SetRotationRadian(const FEVector3& radian)
{
	_qRot = FEQuaternionRotationRollPitchYaw(radian);
}


FEVector3 FETransform::GetPositionWorld()
{
	return Vector3::Zero * GetRotPosMatrix();
}


FEVector3 FETransform::GetPositionLocal()
{
	return _vPos;
}


FEVector4 FETransform::GetRotationQuaternion()
{
	return _qRot;
}



FEVector3 FETransform::GetLookAt()
{
	return FEVector3::Forward * FEMatrixRotationQuaternion(_qRot);
}


FEVector3 FETransform::GetUpVector()
{
	return FEVector3::Up * FEMatrixRotationQuaternion(_qRot);
}


FEVector3 FETransform::GetRightVector()
{
	return FEVector3::Right * FEMatrixRotationQuaternion(_qRot);
}