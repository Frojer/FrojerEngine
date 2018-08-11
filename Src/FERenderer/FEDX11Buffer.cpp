#include "FEDX11Buffer.h"
#include "FEDX11Renderer.h"

FEDX11Buffer::FEDX11Buffer()
{
}

FEDX11Buffer::~FEDX11Buffer()
{
	Release();
}


bool FEDX11Buffer::Create(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = (D3D11_USAGE)usage;		// ���� �����
	bd.ByteWidth = bufferSize;			// ���� ũ��
	bd.BindFlags = bindFlag;			// ���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = cpuAccess ? D3D11_CPU_ACCESS_WRITE : 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = bufferData;			// ���ۿ� �� ������ ���� : "������"..

	// ���� ���� ����.
	hr = ((FEDX11Renderer*)IFERenderer::GetInstance())->GetDevice()->CreateBuffer(&bd, &rd, &_pBuffer);

	if (FAILED(hr))
		return false;

	return true;
}


void FEDX11Buffer::Release()
{
	SAFE_RELEASE(_pBuffer);
}


void FEDX11Buffer::UpdateBuffer()
{

}


ID3D11Buffer* FEDX11Buffer::GetBuffer() const
{
	return _pBuffer;
}