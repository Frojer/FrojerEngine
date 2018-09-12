#pragma once
#ifndef _FE_TEXTURE
#define _FE_TEXTURE

#include "FEObjectHeader.h"
#include <IFETexture.h>

class FETexture : public FEObject
{
private:
	static std::unordered_map<INT64, FETexture*> _textureMap;

	IFETexture* _pTex;

private:
	FETexture(INT64 ID);
	bool CreateTexture(tstring file);

public:
	FETexture();
	virtual ~FETexture();

	static FETexture* Find(INT64 id);
	static FETexture* Find(LPCTSTR name);

	friend class FELoader;
};

#endif