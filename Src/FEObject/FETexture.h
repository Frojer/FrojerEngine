#pragma once
#ifndef _FE_TEXTURE
#define _FE_TEXTURE

#include "FEObjectHeader.h"
#include <IFETexture.h>

class FETexture : public FEObject
{
private:
	static std::unordered_map<INT64, FETexture*> _textureMap;

	FEVector4 _ot;
	FEVector4 _angle_Amount;
	IFETexture* _pTex;

private:
	FETexture(INT64 ID);
	bool CreateTexture(tstring file);

public:
	FETexture();
	virtual ~FETexture();

	FEVector2 GetOffset();
	void SetOffset(FEVector2 offset);

	FEVector2 GetTiling();
	void SetTiling(FEVector2 tiling);

	FEVector3 GetAngle();
	void SetAngle(FEVector3 angle);

	float GetAmount();
	void SetAmount(float amount);

	static FETexture* Find(INT64 id);
	static FETexture* Find(LPCTSTR name);
};

#endif