#include "FEObjectHeader.h"

using namespace std;

unordered_map<INT64, FEMaterial*> FEMaterial::_mtrlMap;
FETexture* FEMaterial::_pDefaultTex = nullptr;
FEMaterial::WVP_Data FEMaterial::_WVPData;
FEMaterial::Light_Data FEMaterial::_LightData[FE_LIGHT_SIZE];

FEMaterial::FEMaterial(INT64 ID, FEShader* i_pShader)
	: FEObject(ID), _pShader(i_pShader), _countTexture(0), m_diffuse(FEVector4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(FEVector3(1.0f, 1.0f, 1.0f)), m_specular(FEVector3(1.0f, 1.0f, 1.0f))
{
	SetShader(i_pShader);

	_mtrlMap[GetID()] = this;
}

FEMaterial::FEMaterial(FEShader* i_pShader)
	: _pShader(i_pShader), _countTexture(0), m_diffuse(FEVector4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(FEVector3(1.0f, 1.0f, 1.0f)), m_specular(FEVector3(1.0f, 1.0f, 1.0f))
{
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



void FEMaterial::SetScalar(const UINT index, const float scalar)
{
	*((float*)&_vecScalar[index / 4] + (index % 4)) = scalar;

	_vecScalarA[index / 4] = FEMath::FEConvertToAlignData(_vecScalar[index / 4]);
}
void FEMaterial::SetScalar(const UINT index, const int scalar)
{
	*((int*)&_vecScalar[index / 4] + (index % 4)) = scalar;

	_vecScalarA[index / 4] = FEMath::FEConvertToAlignData(_vecScalar[index / 4]);
}
void FEMaterial::SetScalar(const UINT index, const UINT scalar)
{
	*((UINT*)&_vecScalar[index / 4] + (index % 4)) = scalar;

	_vecScalarA[index / 4] = FEMath::FEConvertToAlignData(_vecScalar[index / 4]);
}
void FEMaterial::SetScalar(const UINT index, const bool scalar)
{
	*((bool*)&_vecScalar[index / 4] + (index % 4)) = scalar;

	_vecScalarA[index / 4] = FEMath::FEConvertToAlignData(_vecScalar[index / 4]);
}
void FEMaterial::SetVector(const UINT index, const FEVector4& vector)
{
	_vecVector[index] = FEMath::FEConvertToAlignData(vector);
}
void FEMaterial::SetMatrix(const UINT index, const FEMatrix& matrix)
{
	_vecMatrix[index] = FEMath::FEConvertToAlignData(matrix);
}


bool FEMaterial::GetTexture(const UINT index, FETexture*& o_texture) const
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	o_texture = _texInfo[index].pTexture;

	return true;
}
bool FEMaterial::SetTexture(const UINT index, FETexture* i_texture)
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	_texInfo[index].pTexture = i_texture;

	return true;
}
bool FEMaterial::GetTextureOffset(const UINT index, FEVector2& o_offset) const
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	o_offset = FEVector2(_texInfo[index].ot.x, _texInfo[index].ot.y);

	return true;
}
bool FEMaterial::SetTextureOffset(const UINT index, const FEVector2 i_offset)
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	_texInfo[index].ot.x = i_offset.x;
	_texInfo[index].ot.y = i_offset.y;

	return true;
}
bool FEMaterial::GetTextureTiling(const UINT index, FEVector2& o_tiling) const
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	o_tiling = FEVector2(_texInfo[index].ot.z, _texInfo[index].ot.w);

	return true;
}
bool FEMaterial::SetTextureTiling(const UINT index, const FEVector2 i_tiling)
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	_texInfo[index].ot.z = i_tiling.x;
	_texInfo[index].ot.w = i_tiling.y;

	return true;
}
bool FEMaterial::GetTextureAngle(const UINT index, FEVector3& o_angle) const
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	o_angle = FEVector3(_texInfo[index].angle_Amount.x, _texInfo[index].angle_Amount.y, _texInfo[index].angle_Amount.z);

	return true;
}
bool FEMaterial::SetTextureAngle(const UINT index, FEVector3 i_angle)
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	_texInfo[index].angle_Amount.x = i_angle.x;
	_texInfo[index].angle_Amount.y = i_angle.y;
	_texInfo[index].angle_Amount.z = i_angle.z;

	return true;
}
bool FEMaterial::GetTextureAmount(const UINT index, float& o_amount) const
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	o_amount = _texInfo[index].angle_Amount.w;

	return true;
}
bool FEMaterial::SetTextureAmount(const UINT index, float i_amount)
{
	if (index >= FE_TEXTURE_SIZE)	return false;

	_texInfo[index].angle_Amount.w = i_amount;

	return true;
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