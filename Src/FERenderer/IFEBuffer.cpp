#include "IFEBuffer.h"
#ifdef FE_DX11
#include "FEDX11Buffer.h"
#endif

IFEBuffer* IFEBuffer::CreateBuffer(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData, UINT stride, UINT offset)
{
	IFEBuffer* pBuffer = nullptr;

#ifdef FE_DX11
	pBuffer = new FEDX11Buffer;
#endif

	if (pBuffer == nullptr) return nullptr;

	pBuffer->Create(bindFlag, usage, cpuAccess, bufferSize, bufferData);
	pBuffer->stride = stride;
	pBuffer->offset = offset;

	return pBuffer;
}