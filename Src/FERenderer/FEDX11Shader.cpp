#include "FEDX11Shader.h"
#ifdef FE_DX11
#include <stdio.h>
#include <vector>
#include "FEDX11Renderer.h"
#include "FEDX11Texture.h"


////////////////////////////////////////////////////////////////////////////// 
//
// �����ϵ� ���̴��ڵ� �ε� : 
//
// ���̴� �����Ϸ�(fxc.exe) �� ���� ������ ����� ��� �ε��մϴ�.
// VisualStudio 2012 ���ʹ� fxc.exe �� �����ϰ� ����.
// �� �������� ������ �������Ϸ� �����Ǿ����ϴ�.
//
HRESULT ShaderLoadFromFile(
	LPCTSTR FileName,		//�ҽ������̸�.
	BYTE** ppCode,			//[���] �����ϵ� ���̴� �ڵ�
	SIZE_T* pSize			//[���] �����ϵ� ���̴� �ڵ� ũ��.
)
{
	HRESULT hr = S_OK;

	//���̳ʸ� ����.
	FILE* fp = nullptr;
#ifdef _UNICODE
	_wfopen_s(&fp, FileName, L"rb");
#else
	FILE* fp = fopen(FileName, "rb");
#endif
	if (fp == NULL)
	{
		//Debug::WarningMessage(FE_TEXT("���̴� ���� �б� ����"));
		*ppCode = nullptr;
		*pSize = 0;
		return E_FAIL;
	}

	//����ũ�� ȹ��.
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//���̳ʸ� �ϰ� �б�.
	BYTE* pBuff = new BYTE[size];
	fread(pBuff, size, sizeof(BYTE), fp);
	fclose(fp);

	//�ܺη� ����.
	*ppCode = pBuff;
	*pSize = size;

	return hr;
}

FEDX11Shader::FEDX11Shader()
	: _pVS(nullptr), _pPS(nullptr), _pVBLayout(nullptr), _pVSCode(nullptr), _VSCodeSize(0), _PSCodeSize(0)
{
}


FEDX11Shader::~FEDX11Shader()
{
	Release();
}


bool FEDX11Shader::CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName)
{
	FEDX11Renderer* pRenderer = (FEDX11Renderer*)IFERenderer::GetInstance();

	HRESULT hr = S_OK;

	//--------------------------
	// ���� ���̴� ����.
	//--------------------------
	// ���� ���̴� ���̳ʸ� - �����ϵ� ���� �ڵ� - �б�
	hr = ShaderLoadFromFile(i_vsName, &_pVSCode, &_VSCodeSize);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Vertex Shader �ε� ����", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// ���� ���̴� ��ü ����
	hr = pRenderer->GetDevice()->CreateVertexShader(_pVSCode, _VSCodeSize, nullptr,	&_pVS);

	if (FAILED(hr))
	{
		SAFE_DELETE(_pVSCode);
		return false;
	}



	//--------------------------
	// �ȼ� ���̴� ����.
	//--------------------------
	// �ȼ� ���̴� ���̳ʸ� - �����ϵ� ���� �ڵ� - �б�
	BYTE* pPSCode = nullptr;
	hr = ShaderLoadFromFile(i_psName, &pPSCode, &_PSCodeSize);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Pixel Shader �ε� ����", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// �ȼ� ���̴� ��ü ����
	hr = pRenderer->GetDevice()->CreatePixelShader(pPSCode, _PSCodeSize, nullptr, &_pPS);

	SAFE_DELETE(pPSCode);
	if (FAILED(hr))	return false;

	return true;
}


bool FEDX11Shader::CreateInputLayout(FE_SHADER_SEMANTICS i_semanticsFlag)
{
	FEDX11Renderer* pRenderer = (FEDX11Renderer*)IFERenderer::GetInstance();

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	D3D11_INPUT_ELEMENT_DESC desc;

#define log2(x) log(x)/log(2)
	for (UINT i = 0; i < FE_SHADER_SEMANTIC_NUM; i++)
	{
		switch (i_semanticsFlag & (UINT)pow(2U, i))
		{
		case FE_SHADER_SEMANTIC_POSITION:
			desc.SemanticName = "POSITION";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_POSITION));
			desc.AlignedByteOffset = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);
			break;

		case FE_SHADER_SEMANTIC_COLOR:
			desc.SemanticName = "COLOR";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.InputSlot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_COLOR));
			desc.AlignedByteOffset = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);
			break;

		case FE_SHADER_SEMANTIC_NORMAL:
			desc.SemanticName = "NORMAL";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_NORMAL));
			desc.AlignedByteOffset = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);
			break;

		case FE_SHADER_SEMANTIC_TEXCOORD:
			desc.SemanticName = "TEXCOORD";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			desc.InputSlot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_TEXCOORD));
			desc.AlignedByteOffset = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);
			break;
		}
	}
#undef log2

	// ���� �Է±��� ��ü ���� Create the input layout
	HRESULT hr = pRenderer->GetDevice()->CreateInputLayout(layout.data(), layout.size(), _pVSCode, _VSCodeSize, &_pVBLayout);
	if (FAILED(hr))	return false;

	return true;
}


bool FEDX11Shader::Create(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semanticsFlag)
{
	bool result;

	result = CreateShader(i_vsName, i_psName);

	if (!result) return false;

	result = CreateInputLayout(i_semanticsFlag);

	if (!result) return false;

	return result;
}


void FEDX11Shader::Release()
{
	SAFE_RELEASE(_pVBLayout);
	SAFE_RELEASE(_pVS);			//���� ���̴� ����.
	SAFE_RELEASE(_pPS);			//�ȼ� ���̴� ����.
	SAFE_DELETE(_pVSCode);		//���� ���̴� ������ �ڵ� (����) ����
	_VSCodeSize = 0;
	_PSCodeSize = 0;
}


void FEDX11Shader::Render() const
{
	FEDX11Renderer* pRenderer = (FEDX11Renderer*)IFERenderer::GetInstance();
	pRenderer->GetDXDC()->IASetInputLayout(_pVBLayout);
	pRenderer->GetDXDC()->VSSetShader(_pVS, nullptr, 0);
	pRenderer->GetDXDC()->PSSetShader(_pPS, nullptr, 0);
}


void FEDX11Shader::SetConstantBuffer(UINT StartSlot, IFEBuffer* pConstantBuffers)
{
	FEDX11Renderer* pRenderer = (FEDX11Renderer*)IFERenderer::GetInstance();

	auto pBuf = static_cast<FEDX11Buffer*>(pConstantBuffers)->GetBuffer();

	pRenderer->GetDXDC()->VSSetConstantBuffers(StartSlot, 1, &pBuf);
	pRenderer->GetDXDC()->PSSetConstantBuffers(StartSlot, 1, &pBuf);
}
void FEDX11Shader::SetShaderResource(UINT StartSlot, IFETexture* pTexture)
{
	FEDX11Renderer* pRenderer = (FEDX11Renderer*)IFERenderer::GetInstance();

	auto pShaderResource = static_cast<FEDX11Texture*>(pTexture)->GetShaderResourceView();

	pRenderer->GetDXDC()->VSSetShaderResources(StartSlot, 1, &pShaderResource);
	pRenderer->GetDXDC()->PSSetShaderResources(StartSlot, 1, &pShaderResource);
}
#endif