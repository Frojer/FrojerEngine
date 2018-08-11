#pragma once
#ifndef _FE_SHADER
#define _FE_SHADER

#include <IFEShader.h>
#include "FEObjectHeader.h"

class FEShader : public FEObject
{
private:
	IFEShader* _pShader;

public:
	FEShader();
	virtual ~FEShader();

	void Release();

	void Render();
};

#endif