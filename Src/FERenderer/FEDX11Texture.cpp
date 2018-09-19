#include "FEDX11Texture.h"
#ifdef FE_DX11
#include "FEDX11Renderer.h"
#include <FEUtility.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

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

	if (extension == FE_TEXT("jpg") || extension == FE_TEXT("png") || extension == FE_TEXT("bmp") || extension == FE_TEXT("tga"))
	{
		// Mip-map 텍스처 생성
		hr = DirectX::CreateWICTextureFromFileEx(pRenderer->GetDevice(), pRenderer->GetDXDC(), filename, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, DirectX::WIC_LOADER_DEFAULT,
			nullptr, &_pTextureRV);

		if (FAILED(hr))
			hr = DirectX::CreateWICTextureFromFile(pRenderer->GetDevice(), filename, nullptr, &_pTextureRV);
	}

	else if (extension == FE_TEXT("dds"))
	{
		hr = DirectX::CreateDDSTextureFromFileEx(pRenderer->GetDevice(), pRenderer->GetDXDC(), filename, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false,
			nullptr, &_pTextureRV);

		if (FAILED(hr))
			hr = DirectX::CreateDDSTextureFromFile(pRenderer->GetDevice(), filename, nullptr, &_pTextureRV);
	}

	else return false;

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