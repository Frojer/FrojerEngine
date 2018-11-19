#include "FECamera.h"
#include <IFERenderer.h>
#include <FEMath.h>

std::vector<FECamera*> FECamera::_vCam;

FECamera::FECamera()
	: m_FOV(FEConvertToRadian(45.0f)), m_Aspect((float)IFERenderer::GetInstance()->m_setting.uiWindowWidth / IFERenderer::GetInstance()->m_setting.uiWindowHeight), m_Znear(1.0f), m_Zfar(1000.0f)
{
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = (float)IFERenderer::GetInstance()->m_setting.uiWindowWidth;
	m_viewport.Height = (float)IFERenderer::GetInstance()->m_setting.uiWindowHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	FECamera::_vCam.push_back(this);
}


FECamera::~FECamera()
{
	FOR_STL(FECamera::_vCam)
	{
		if ((*iter) == this)
		{
			FECamera::_vCam.erase(iter);
			break;
		}
	}
}


FEMatrix FECamera::GetViewMatrixLH()
{
	FEGameObject* pObj = GetMyObject();

	FEVector3 pos = pObj->GetTransform()->GetPositionWorld();
	FEVector3 lookAt = pObj->GetTransform()->GetLookAt() + pObj->GetTransform()->GetPositionWorld();
	FEVector3 up = pObj->GetTransform()->GetUpVector();

	//MATRIX mView;
	//XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, lookAt, up));

	//return mView;

	return FEMatrixLookAtLH(pos, lookAt, up);
}
FEMatrix FECamera::GetPerspectiveFovLH()
{
	//MATRIX mProj;
	//XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar));

	//return mProj;

	return FEMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar);
}


void FECamera::LookAt(FEVector3 target)
{

}


FECamera* FECamera::GetCamera(UINT index)
{
	return FECamera::_vCam[index];
}
std::vector<FECamera*> FECamera::GetAllCameras()
{
	return FECamera::_vCam;
}
UINT FECamera::GetAllCamerasCount()
{
	return FECamera::_vCam.size();
}