#include <IFEBuffer.h>
#include "FEObjectHeader.h"

std::unordered_map<INT64, FEShader*> FEShader::_shaderMap;
IFEBuffer* FEShader::_pLightCB = nullptr;
IFEBuffer* FEShader::_pPerCamCB = nullptr;
IFEBuffer* FEShader::_pPerMtrlCB = nullptr;
IFEBuffer* FEShader::_pPerObjCB = nullptr;

FEShader::FEShader(INT64 ID)
	: FEObject(ID), _pConstBuffer(nullptr), _pShader(nullptr), _countMatrix(0), _countVector(0), _countScalar(0)
{
	_shaderMap[GetID()] = this;
}

FEShader::FEShader()
	: _pConstBuffer(nullptr), _pShader(nullptr), _countMatrix(0), _countVector(0), _countScalar(0)
{
	_shaderMap[GetID()] = this;
}


FEShader::~FEShader()
{
	_shaderMap.erase(GetID());

	Release();
}


void FEShader::ClearMap()
{
	auto i = _shaderMap.begin();

	while (i != _shaderMap.end())
		delete (i++)->second;
}


bool FEShader::CreateDefaultConstantBuffer()
{
	_pLightCB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(FEMaterial::_lightCB), FEMaterial::_lightCB);
	if (_pLightCB == nullptr) return false;
	_pPerCamCB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(FEMaterial::_perCamCB), &FEMaterial::_perCamCB);
	if (_pPerCamCB == nullptr) return false;
	_pPerMtrlCB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(FEMaterial::_perMtrlCB), &FEMaterial::_perMtrlCB);
	if (_pPerMtrlCB == nullptr) return false;
	_pPerObjCB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(FEMaterial::_perObjCB), &FEMaterial::_perObjCB);
	if (_pPerObjCB == nullptr) return false;

	return true;
}
void FEShader::ReleaseDefaultConstantBuffer()
{
	SAFE_DELETE(_pLightCB);
	SAFE_DELETE(_pPerCamCB);
	SAFE_DELETE(_pPerMtrlCB);
	SAFE_DELETE(_pPerObjCB);
}
void FEShader::UpdateConstantBufferLight()
{
	FEShader::_pLightCB->UpdateBuffer(FEMaterial::_lightCB, sizeof(FEMaterial::_lightCB));
}
void FEShader::UpdateConstantBufferPerCamera()
{
	FEShader::_pPerCamCB->UpdateBuffer(&FEMaterial::_perCamCB, sizeof(FEMaterial::_perCamCB));
}


bool FEShader::CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semantics)
{
	_pShader = IFEShader::CreateShader(i_vsName, i_psName, i_semantics);

	_semantics = i_semantics;

	if (_pShader == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to IFEShader::CreateShader()"));
		return false;
	}

	CreateConstantBuffer();

	return true;
}


bool FEShader::CreateConstantBuffer()
{
	//FEVectorA* pData;
	UINT totalSize;

	totalSize = (_countMatrix * 4) + _countVector + ((_countScalar / 4) + (_countScalar % 4 == 0 ? 0 : 1));

	if (totalSize == 0) return true;

	_pConstBuffer = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(FEVectorA) * totalSize, nullptr);

	if (_pConstBuffer == nullptr) return false;

	//if (totalSize != 0)
	//{
	//	// 그래픽 메모리를 사용할것이기 때문에 16바이트 정렬된 메모리로 할당 받는다 ( aligned_malloc(), aligned_free() )
	//	pData = (FEVectorA*)_aligned_malloc(sizeof(FEVectorA) * totalSize, 16);
	//	pData = new VECTOR[totalSize];
	//	memset(pData, 0, sizeof(FEVectorA) * totalSize);
	//	CreateDynamicConstantBuffer(totalSize * sizeof(VECTOR), pData, &i_pShader->_pConstBuffer);
	//	delete[] pData;
	//	_aligned_free(pData);
	//}

	return true;
}
void FEShader::UpdateConstantBufferPerMaterial()
{
	FEShader::_pPerMtrlCB->UpdateBuffer(&FEMaterial::_perMtrlCB, sizeof(FEMaterial::_perMtrlCB));
}
void FEShader::UpdateConstantBufferPerObject()
{
	FEShader::_pPerObjCB->UpdateBuffer(&FEMaterial::_perObjCB, sizeof(FEMaterial::_perObjCB));
}
void FEShader::UpdateConstantBuffer(const void* pCB, UINT size)
{
	_pConstBuffer->UpdateBuffer(pCB, size);
}
void FEShader::SetShaderResources(UINT StartSlot, FETexture* pTexture)
{
	_pShader->SetShaderResource(StartSlot, pTexture->_pTex);
}


void FEShader::Release()
{
	SAFE_DELETE(_pConstBuffer);
	SAFE_DELETE(_pShader);
}


void FEShader::Render() const
{
	UINT i = 0;

	_pShader->Render();

	_pShader->SetConstantBuffer(i++, _pLightCB);
	_pShader->SetConstantBuffer(i++, _pPerCamCB);
	_pShader->SetConstantBuffer(i++, _pPerMtrlCB);
	_pShader->SetConstantBuffer(i++, _pPerObjCB);
	if (_pConstBuffer != nullptr)	_pShader->SetConstantBuffer(i++, _pConstBuffer);
}


FEShader* FEShader::Find(INT64 id)
{
	if (_shaderMap.find(id) == _shaderMap.end())
		return nullptr;

	return _shaderMap[id];
}


FEShader* FEShader::Find(LPCTSTR name)
{
	FOR_STL(_shaderMap)
	{
		if (iter->second->m_Name == name)
			return iter->second;
	}

	return nullptr;
}