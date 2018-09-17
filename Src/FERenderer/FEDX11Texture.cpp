#include "FEDX11Texture.h"
#ifdef FE_DX11
#include "FEDX11Renderer.h"
#include <FEUtility.h>
#include <WICTextureLoader.h>

#pragma comment(lib, "DirectXTK")


FEDX11Texture::FEDX11Texture()
	: _pTextureRV(nullptr)
{
}


FEDX11Texture::~FEDX11Texture()
{
}


bool FEDX11Texture::Create(LPCTSTR filename)
{
	HRESULT hr;
	FEDX11Renderer* pRenderer = static_cast<FEDX11Renderer*>(IFERenderer::GetInstance());

	tstring extension;
	extension = GetFileNameExtension(filename);

	//if (extension == FE_TEXT("jpg") || extension == FE_TEXT("png"))
		hr = DirectX::CreateWICTextureFromFile(pRenderer->GetDevice(), filename, nullptr, &_pTextureRV);

	//else if (extension == FE_TEXT("dds"))
	//	hr = DirectX::CreateDDSTextureFromFile(pRenderer->GetDevice(), filename, nullptr, &_pTextureRV);

	//else return false;

	if (FAILED(hr))
	{
		//FEDebug::ErrorMessage(FE_TEXT("텍스처 로드 실패"));
		return false;
	}

	return true;
}


void FEDX11Texture::Render(UINT startSlot) const
{
	FEDX11Renderer* pRenderer = static_cast<FEDX11Renderer*>(IFERenderer::GetInstance());

	pRenderer->GetDXDC()->VSSetShaderResources(startSlot, 1, &_pTextureRV);
	pRenderer->GetDXDC()->PSSetShaderResources(startSlot, 1, &_pTextureRV);
}
ID3D11ShaderResourceView* FEDX11Texture::GetShaderResourceView() const
{
	return _pTextureRV;
}
#endif