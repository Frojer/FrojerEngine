#pragma once
#ifndef _FE_CAMERA
#define _FE_CAMERA

#include "FEObjectHeader.h"
#include <IFERenderer.h>

class FECamera : public FEBehaviour
{
private:
	static std::vector<FECamera*> _vCam;

public:
	FEViewport m_viewport;

	float m_FOV;
	float m_Aspect;

	float m_Znear;
	float m_Zfar;

public:
	FECamera();
	virtual ~FECamera();

	FEMatrix GetViewMatrixLH();
	FEMatrix GetPerspectiveFovLH();

	void LookAt(FEVector3 target);

	static FECamera* GetCamera(UINT index);
	static std::vector<FECamera*> GetAllCameras();
	static UINT GetAllCamerasCount();
};
#endif