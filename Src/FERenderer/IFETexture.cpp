#include "IFERenderer.h"
#include "IFETexture.h"
#include "FEDX11Texture.h"

IFETexture* IFETexture::CreateTexture(LPCTSTR filename)
{
	IFETexture* pTex = nullptr;

	switch (IFERenderer::GetInstance()->GetPlatform())
	{
	case FE_DX11:
		pTex = new FEDX11Texture;
	}

	if (pTex == nullptr) return nullptr;

	pTex->Create(filename);

	return pTex;
}