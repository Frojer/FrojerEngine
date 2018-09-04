#include "FEObjectHeader.h"

using namespace std;

unordered_map<INT64, FEMaterial*> FEMaterial::_mtrlMap;
FETexture* FEMaterial::_pDefaultTex = nullptr;
FEMaterial::WVP_Data FEMaterial::_WVPData;
FEMaterial::Light_Data FEMaterial::_LightData[LIGHT_SIZE];

FEMaterial::FEMaterial(FEShader* i_pShader)
	: _pShader(i_pShader), _countTexture(0), m_diffuse(FEVector4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(FEVector3(1.0f, 1.0f, 1.0f)), m_specular(FEVector3(1.0f, 1.0f, 1.0f))
{
	memset(m_pTexture, 0, sizeof(m_pTexture));

	SetShader(i_pShader);

	_mtrlMap[GetID()] = this;
}


FEMaterial::~FEMaterial()
{
	_mtrlMap.erase(GetID());
}


void FEMaterial::ClearMap()
{
	auto i = _mtrlMap.begin();

	while (i != _mtrlMap.end())
		delete (i++)->second;
}


void FEMaterial::UpdateLightData()
{
//	auto iter = Light::_lightList.begin();
//	FEVector3 vec;
//
//	//memset(_LightData, 0, sizeof(_LightData));
//
//	for (UINT i = 0; iter != Light::_lightList.end() && i < LIGHT_SIZE; iter++)
//	{
//		if (!(*iter)->GetEnable())
//			continue;
//#ifdef _WIN32
//		using namespace DirectX;
//
//		_LightData[i].diffuse = DirectX::XMLoadFloat4(&(*iter)->m_diffuse);
//		_LightData[i].ambient = XMLoadFloat4(&(*iter)->m_ambient);
//		_LightData[i].specular = XMLoadFloat4(&(*iter)->m_specular);
//		_LightData[i].position = XMLoadFloat3(&(*iter)->GetMyObject()->m_pTransform->GetPositionWorld());
//		vec = (*iter)->GetMyObject()->m_pTransform->GetRotationRadian();
//		_LightData[i].direction = -XMVector3Transform(XMLoadFloat3(&FEVector3(0.0f, 0.0f, 1.0f)), XMMatrixRotationRollPitchYaw(vec.x, vec.y, vec.z));
//		_LightData[i].range = (*iter)->m_range;
//		_LightData[i].lightType = (*iter)->m_lightType;
//		_LightData[i].useLight = true;
//#elif
//
//#endif
//		++i;
//	}
}


void FEMaterial::UpdateConstantBuffer(FEMatrixA& mWorld)
{
	FEMaterial::_WVPData.mWorld = mWorld;
	FEMaterial::_WVPData.mWV = mWorld * FEMaterial::_WVPData.mView;

	if (_constData.size() > 0)
	{
		if (_vecMatrix.size() > 0)
			memcpy_s(&_constData[0], sizeof(FEMatrixA) * _vecMatrix.size(), &_vecMatrix[0], sizeof(FEMatrixA) * _vecMatrix.size());
		if (_vecVector.size() > 0)
			memcpy_s(&_constData[_vecMatrix.size() * 4], sizeof(FEVectorA) * _vecVector.size(), &_vecVector[0], sizeof(FEVectorA) * _vecVector.size());
		if (_vecScalarA.size() > 0)
			memcpy_s(&_constData[(_vecMatrix.size() * 4) + _vecVector.size()], sizeof(FEVectorA) * _vecScalarA.size(), &_vecScalarA[0], sizeof(FEVectorA) * _vecScalarA.size());

		/*if (_vecScalarA.size() > 0)
		memcpy_s(&_constData[0], sizeof(FEVectorA) * _vecScalarA.size(), &_vecScalarA[0], sizeof(FEVectorA) * _vecScalarA.size());
		if (_vecVector.size() > 0)
		memcpy_s(&_constData[_vecScalarA.size()], sizeof(FEVectorA) * _vecVector.size(), &_vecVector[0], sizeof(FEVectorA) * _vecVector.size());
		if (_vecMatrix.size() > 0)
		memcpy_s(&_constData[_vecScalarA.size() + _vecVector.size()], sizeof(FEMatrixA) * _vecMatrix.size(), &_vecMatrix[0], sizeof(FEMatrixA) * _vecMatrix.size());*/

		_pShader->UpdateConstantBuffer(&_constData[0], _constData.size() * sizeof(FEVectorA));
	}

	else
		_pShader->UpdateConstantBuffer(nullptr, 0);
}


void FEMaterial::Render()
{
	if (_pShader == nullptr)
		return;

	//auto sampler = FETexture::GetSampler(1);
	//
	//// ¼ÀÇÃ·¯ ¼³Á¤
	//_pShader->_pDXDC->PSSetSamplers(0, 1, &sampler);
	//
	//for (UINT i = 0; i < _countTexture; i++)
	//{
	//	// ¼ÎÀÌ´õ ¸®¼Ò½º ¼³Á¤.
	//	if (m_pTexture[i] == nullptr)
	//		_pShader->_pDXDC->PSSetShaderResources(i, 1, _pDefaultTex->GetResourceView());
	//	else
	//		_pShader->_pDXDC->PSSetShaderResources(i, 1, m_pTexture[i]->GetResourceView());
	//}

	_pShader->Render();
}


void FEMaterial::SetShader(FEShader* i_pShader)
{
	UINT totalSize = 0;
	UINT scalarSize = 0;

	if (i_pShader == nullptr)
	{
		// Default Shader¸¦ »ðÀÔ
		_pShader = FEShader::Find(L"Error");
	}

	else _pShader = i_pShader;

	scalarSize = (_pShader->_countScalar / 4) + (_pShader->_countScalar % 4 == 0 ? 0 : 1);

	_countTexture = _pShader->_countTexture;
	totalSize += _pShader->_countMatrix * 4;
	totalSize += _pShader->_countVector;
	totalSize += scalarSize;

	_constData.resize(totalSize);
	_vecScalar.resize(scalarSize);
	_vecScalarA.resize(scalarSize);
	_vecVector.resize(_pShader->_countVector);
	_vecMatrix.resize(_pShader->_countMatrix);

	/*if (totalSize != 0)
	{
	_constData.resize(totalSize);

	if (_pShader->_countScalar != 0)
	{
	_vecScala.resize(_pShader->_countScalar);
	ZeroMemory(&_vecScala[0], _vecScala.size() * sizeof(VECTOR));
	}

	if (_pShader->_countVector != 0)
	{
	_vecVector.resize(_pShader->_countVector);
	ZeroMemory(&_vecVector[0], _vecVector.size() * sizeof(VECTOR));
	}

	if (_pShader->_countMatrix != 0)
	{
	_vecMatrix.resize(_pShader->_countMatrix);
	ZeroMemory(&_vecMatrix[0], _vecMatrix.size() * sizeof(MATRIXA));
	}
	}*/
}


FEShader* FEMaterial::GetShader() const
{
	return _pShader;
}



void FEMaterial::SetScalar(UINT id, const float scalar)
{
	*((float*)&_vecScalar[id / 4] + (id % 4)) = scalar;

	_vecScalarA[id / 4] = FEMath::FEConvertToAlignData(_vecScalar[id / 4]);
}



void FEMaterial::SetScalar(UINT id, const int scalar)
{
	*((int*)&_vecScalar[id / 4] + (id % 4)) = scalar;

	_vecScalarA[id / 4] = FEMath::FEConvertToAlignData(_vecScalar[id / 4]);
}



void FEMaterial::SetScalar(UINT id, const UINT scalar)
{
	*((UINT*)&_vecScalar[id / 4] + (id % 4)) = scalar;

	_vecScalarA[id / 4] = FEMath::FEConvertToAlignData(_vecScalar[id / 4]);
}



void FEMaterial::SetScalar(UINT id, const bool scalar)
{
	*((bool*)&_vecScalar[id / 4] + (id % 4)) = scalar;

	_vecScalarA[id / 4] = FEMath::FEConvertToAlignData(_vecScalar[id / 4]);
}



void FEMaterial::SetVector(UINT id, const FEVector4& vector)
{
	_vecVector[id] = FEMath::FEConvertToAlignData(vector);
}



void FEMaterial::SetMatrix(UINT id, const FEMatrix& matrix)
{
	_vecMatrix[id] = FEMath::FEConvertToAlignData(matrix);
}



FEMaterial* FEMaterial::Find(INT64 id)
{
	if (_mtrlMap.find(id) == _mtrlMap.end())
		return nullptr;

	return _mtrlMap[id];
}


FEMaterial* FEMaterial::Find(LPCTSTR name)
{
	FOR_STL(_mtrlMap)
	{
		if (iter->second->m_Name == name)
			return iter->second;
	}

	return nullptr;
}