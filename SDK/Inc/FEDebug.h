#pragma once
#ifndef _FE_DEBUG
#define _FE_DEBUG

#include <FEMath.h>
#include <FEDefine.h>
#include <IFEBuffer.h>
#include <IFEShader.h>
#include <FEObjectHeader.h>
#include <queue>

struct PointCB
{
	FEMatrixA mVP;
	FEVectorA vColor;
};

class FEDebug
{
private:
	struct Line
	{
		FEVector3 pos[2];
		FEVector4 color;
	};
	static FEDebug* _pInstance;

	static PointCB _PointCBData;
	IFEBuffer* _pLineVB;
	IFEBuffer* _pLineCB;
	IFEShader* _pLineShader;
	std::queue<Line> _lineDrawQueue;

private:
	FEDebug();

	bool Initialize();
	void Release();

	void RenderLineDrawQueue();

public:
	~FEDebug();

	static FEDebug* GetInstance();

	void DrawLine(FEVector3 start, FEVector3 end, FEVector4 color);
	void DrawNormal(FEGameObject* pObj, FEVector4 col);

	void ErrorMessage(tstring text);
	void WarningMessage(tstring text);

	friend class FESystem;
};
#endif