#include "IFETexture.h"
#ifdef FE_DX11
#include "FEDX11Texture.h"
#endif

IFETexture* IFETexture::CreateTexture(LPCTSTR filename)
{
	IFETexture* pTex = nullptr;

#ifdef FE_DX11
	pTex = new FEDX11Texture;
#endif

	if (pTex == nullptr) return nullptr;

	pTex->Create(filename);

	return pTex;
}