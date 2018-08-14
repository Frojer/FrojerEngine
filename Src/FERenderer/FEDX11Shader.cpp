#include <stdio.h>
#include <vector>

#include "FEDX11Shader.h"
#include "FEDX11Renderer.h"


////////////////////////////////////////////////////////////////////////////// 
//
// 컴파일된 셰이더코드 로딩 : 
//
// 셰이더 컴파일러(fxc.exe) 를 통해 사전에 빌드된 기계어를 로딩합니다.
// VisualStudio 2012 부터는 fxc.exe 를 내장하고 있음.
// 그 이전에는 별도의 실행파일로 배포되었습니다.
//
HRESULT ShaderLoadFromFile(
	LPCTSTR FileName,		//소스파일이름.
	BYTE** ppCode,			//[출력] 컴파일된 셰이더 코드
	SIZE_T* pSize			//[출력] 컴파일된 셰이더 코드 크기.
)
{
	HRESULT hr = S_OK;

	//바이너리 열기.
	FILE* fp = nullptr;
#ifdef _UNICODE
	_wfopen_s(&fp, FileName, L"rb");
#else
	FILE* fp = fopen(FileName, "rb");
#endif
	if (fp == NULL)
	{
		//Debug::WarningMessage(FE_TEXT("셰이더 파일 읽기 실패"));
		*ppCode = nullptr;
		*pSize = 0;
		return E_FAIL;
	}

	//파일크기 획득.
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//바이너리 일괄 읽기.
	BYTE* pBuff = new BYTE[size];
	fread(pBuff, size, sizeof(BYTE), fp);
	fclose(fp);

	//외부로 리턴.
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
	// 정점 셰이더 생성.
	//--------------------------
	// 정점 셰이더 바이너리 - 컴파일된 기계어 코드 - 읽기
	hr = ShaderLoadFromFile(i_vsName, &_pVSCode, &_VSCodeSize);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Vertex Shader 로드 실패", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// 정점 셰이더 객체 생성
	hr = pRenderer->GetDevice()->CreateVertexShader(_pVSCode, _VSCodeSize, nullptr,	&_pVS);

	if (FAILED(hr))
	{
		SAFE_DELETE(_pVSCode);
		return false;
	}



	//--------------------------
	// 픽셀 셰이더 생성.
	//--------------------------
	// 픽셀 셰이더 바이너리 - 컴파일된 기계어 코드 - 읽기
	BYTE* pPSCode = nullptr;
	hr = ShaderLoadFromFile(i_psName, &pPSCode, &_PSCodeSize);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Pixel Shader 로드 실패", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// 픽셀 셰이더 객체 생성
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
	UINT offset = 0;
	
	for (UINT i = 0; i < sizeof(i_semanticsFlag) * 8; i++)
	{
		switch (i_semanticsFlag & (2 ^ i))
		{
		case FE_SHADER_SEMANTIC_POSITION:
			desc.SemanticName = "POSITION";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);

			offset += 12;
			break;

		case FE_SHADER_SEMANTIC_COLOR:
			desc.SemanticName = "COLOR";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);

			offset += 16;
			break;

		case FE_SHADER_SEMANTIC_NORMAL:
			desc.SemanticName = "NORMAL";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);

			offset += 12;
			break;

		case FE_SHADER_SEMANTIC_TEXCOORD:
			desc.SemanticName = "TEXCOORD";
			desc.SemanticIndex = 0;
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			layout.push_back(desc);

			offset += 8;
			break;
		}
	}

	// 정접 입력구조 객체 생성 Create the input layout
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
	SAFE_RELEASE(_pVS);			//정점 셰이더 제거.
	SAFE_RELEASE(_pPS);			//픽셀 셰이더 제거.
	SAFE_DELETE(_pVSCode);		//정점 셰이더 컴파일 코드 (버퍼) 제거
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