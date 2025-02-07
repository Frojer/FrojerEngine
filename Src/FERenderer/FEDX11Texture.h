#pragma once
#ifndef _FE_DX11_TEXTURE
#define _FE_DX11_TEXTURE

#include "IFETexture.h"
#ifdef FE_DX11
#include <d3d11.h>

class FEDX11Texture : public IFETexture
{
private:
	ID3D11ShaderResourceView* _pTextureRV;

protected:
	virtual bool Create(LPCTSTR filename) override;

public:
	FEDX11Texture();
	virtual ~FEDX11Texture();

	virtual void Render(UINT startSlot) const override;

	ID3D11ShaderResourceView* GetShaderResourceView() const;
};
#endif
#endif