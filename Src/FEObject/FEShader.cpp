#include <IFEBuffer.h>
#include "FEObjectHeader.h"

std::unordered_map<UINT, FEShader*> FEShader::_shaderMap;
IFEBuffer* FEShader::_pWVP_CB = nullptr;
IFEBuffer* FEShader::_pLight_CB = nullptr;

FEShader::FEShader()
	: _pConstBuffer(nullptr), _pShader(nullptr), _countTexture(0), _countMatrix(0), _countVector(0), _countScalar(0), _useLight(false)
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
	_pWVP_CB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DEFAULT, false, sizeof(FEMaterial::_WVPData), &FEMaterial::_WVPData);
	if (_pWVP_CB == nullptr) return false;
	_pLight_CB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DEFAULT, false, sizeof(FEMaterial::_LightData), &FEMaterial::_LightData);
	if (_pLight_CB == nullptr) return false;

	return true;
}


void FEShader::ReleaseDefaultConstantBuffer()
{
	SAFE_DELETE(_pWVP_CB);
	SAFE_DELETE(_pLight_CB);
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


void FEShader::UpdateConstantBuffer(const void* pCB, UINT size)
{
	FEShader::_pWVP_CB->UpdateBuffer(&FEMaterial::_WVPData, sizeof(FEMaterial::_WVPData));
	if (_useLight)	FEShader::_pLight_CB->UpdateBuffer(&FEMaterial::_LightData, sizeof(FEMaterial::_LightData));
	if (size)		_pConstBuffer->UpdateBuffer(pCB, size);
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

	_pShader->SetConstantBuffer(i++, _pWVP_CB);
	if (_useLight)					_pShader->SetConstantBuffer(i++, _pLight_CB);
	if (_pConstBuffer != nullptr)	_pShader->SetConstantBuffer(i++, _pConstBuffer);
}


FEShader* FEShader::Find(UINT id)
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