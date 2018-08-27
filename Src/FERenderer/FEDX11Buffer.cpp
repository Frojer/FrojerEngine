#include "FEDX11Buffer.h"
#include "FEDX11Renderer.h"

FEDX11Buffer::FEDX11Buffer()
	: _pBuffer(nullptr)
{
}

FEDX11Buffer::~FEDX11Buffer()
{
	Release();
}


bool FEDX11Buffer::Create(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData)
{
	HRESULT hr = S_OK;

	_usage = usage;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = (D3D11_USAGE)usage;		// 버퍼 사용방식
	bd.ByteWidth = bufferSize;			// 버퍼 크기
	bd.BindFlags = bindFlag;			// 버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = cpuAccess ? D3D11_CPU_ACCESS_WRITE : 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = bufferData;			// 버퍼에 들어갈 데이터 설정 : "정점들"..

	// 정점 버퍼 생성.
	hr = ((FEDX11Renderer*)IFERenderer::GetInstance())->GetDevice()->CreateBuffer(&bd, &rd, &_pBuffer);

	if (FAILED(hr))
		return false;

	return true;
}


void FEDX11Buffer::Release()
{
	SAFE_RELEASE(_pBuffer);
}


void FEDX11Buffer::UpdateBuffer(const void* pData, UINT size)
{
	FEDX11Renderer* _pRenderer = static_cast<FEDX11Renderer*>(IFERenderer::GetInstance());
	HRESULT hr = S_OK;

	switch (_usage)
	{
	case FE_USAGE_DEFAULT:
		_pRenderer->GetDXDC()->UpdateSubresource(_pBuffer, 0, nullptr, &pData, 0, 0);
		break;

	case FE_USAGE_DYNAMIC:
		D3D11_MAPPED_SUBRESOURCE mr;
		ZeroMemory(&mr, sizeof(mr));

		//상수버퍼 접근
		hr = _pRenderer->GetDXDC()->Map(_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		if (FAILED(hr))
		{
			return;
		}

		//상수 버퍼 갱신.
		memcpy(mr.pData, pData, size);

		//상수버퍼 닫기.
		_pRenderer->GetDXDC()->Unmap(_pBuffer, 0);
		break;
	}
}


ID3D11Buffer* FEDX11Buffer::GetBuffer() const
{
	return _pBuffer;
}