#pragma once
#ifndef _FE_DEBUG
#define _FE_DEBUG

#include <FEMath.h>
#include <FEDefine.h>
#include <IFEBuffer.h>
#include <IFEShader.h>

struct PointCB
{
	FEMatrixA mVP;
	FEVectorA vColor;
};

class FEDebug
{
private:
	static FEDebug* _pInstance;

	static PointCB _PointCBData;
	IFEBuffer* _pLineVB;
	IFEBuffer* _pLineCB;
	IFEShader* _pLineShader;

private:
	FEDebug();

	bool Initialize();
	void Release();

public:
	~FEDebug();

	static FEDebug* GetInstance();

	void DrawLine(FEVector3 start, FEVector3 end, FEVector4 color);

	void ErrorMessage(tstring text);
	void WarningMessage(tstring text);
};
#endif