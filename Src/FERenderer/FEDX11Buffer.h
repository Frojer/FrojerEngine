#pragma once
#ifndef _FE_DX11_BUFFER
#define _FE_DX11_BUFFER

#include <d3d11.h>
#include "IFEBuffer.h"

class FEDX11Buffer : public IFEBuffer
{
private:
	ID3D11Buffer* _pBuffer;
	FE_USAGE _usage;

protected:
	virtual bool Create(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData = nullptr) override;

public:
	FEDX11Buffer();
	virtual ~FEDX11Buffer();

	void Release();

	virtual void UpdateBuffer(const void* pData, UINT size) override;

	ID3D11Buffer* GetBuffer() const;
};

#endif