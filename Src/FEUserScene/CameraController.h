#pragma once

#include <FEObjectHeader.h>

class CameraController : public FEBehaviour
{
public:
	FECamera* pCam;

public:
	CameraController() = default;
	virtual ~CameraController() = default;

	virtual void Update() override;
};

