#include "FEMaterial.h"

using namespace std;
using namespace FEMath;

unordered_map<INT64, FEMaterial*> FEMaterial::_mtrlMap;
FETexture* FEMaterial::_pDefaultTex = nullptr;
FEMaterial::LightCB FEMaterial::_lightCB[FE_LIGHT_SIZE];
FEMaterial::PerCameraCB FEMaterial::_perCamCB;
FEMaterial::PerMaterialCB FEMaterial::_perMtrlCB;
FEMaterial::PerObjectCB FEMaterial::_perObjCB;
INT64 FEMaterial::_oldDrawID = 0;

FEMaterial::FEMaterial(INT64 ID, FEShader* i_pShader)
	: FEObject(ID), _pShader(i_pShader), m_diffuse(FEVector4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(FEVector3(1.0f, 1.0f, 1.0f)), m_specular(FEVector3(1.0f, 1.0f, 1.0f))
{
	SetShader(i_pShader);

	_mtrlMap[GetID()] = this;
}

FEMaterial::FEMaterial(FEShader* i_pShader)
	: _pShader(i_pShader), m_diffuse(FEVector4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(FEVector3(1.0f, 1.0f, 1.0f)), m_specular(FEVector3(1.0f, 1.0f, 1.0f))
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

void FEMaterial::UpdateConstantBufferLight()
{
	// ����Ʈ ������ ������Ʈ
	auto iter = FELight::_lightList.begin();
	FEVector3 vec;

	for (UINT i = 0; iter != FELight::_lightList.end() && i < FE_LIGHT_SIZE; iter++)
	{
		FEMaterial::_lightCB[i].useLight = (*iter)->GetEnable();

		if (!FEMaterial::_lightCB[i].useLight)
			continue;

		FEMaterial::_lightCB[i].diffuse = FEMath::FEConvertToAlignData((*iter)->m_diffuse);
		FEMaterial::_lightCB[i].ambient = FEMath::FEConvertToAlignData((*iter)->m_ambient);
		FEMaterial::_lightCB[i].specular = FEMath::FEConvertToAlignData((*iter)->m_specular);
		FEMaterial::_lightCB[i].position = FEMath::FEConvertToAlignData(FEVector4((*iter)->GetMyObject()->GetTransform()->GetPositionWorld(), 1));
		vec = (*iter)->GetMyObject()->GetTransform()->GetRotationRadian();
		FEMaterial::_lightCB[i].direction = FEMath::FEConvertToAlignData(FEVector4(-(FEVector3::Forward * FEMath::FEMatrixRotationRollPitchYaw(vec)), 0));
		FEMaterial::_lightCB[i].range = (*iter)->m_range;
		FEMaterial::_lightCB[i].lightType = (*iter)->m_lightType;

		++i;
	}

	// ���̴� ������� ������Ʈ
	FEShader::UpdateConstantBufferLight();
}
void FEMaterial::UpdateConstantBufferPerCamera(const FEMatrixA& mView, const FEMatrixA& mProj)
{
	// �� ��� ������Ʈ
	FEMaterial::_perCamCB.mView = mView;
	// �������� ��� ������Ʈ
	FEMaterial::_perCamCB.mProj = mProj;
	// ���̴� ������� ������Ʈ
	FEShader::UpdateConstantBufferPerCamera();
}
void FEMaterial::UpdateConstantBufferPerMaterial()
{
	// �̹� ������ ����� ���׸����̶�� �̹� ���� ��������̹Ƿ� ������Ʈ�� ���� �ʴ´�
	if (GetID() != _oldDrawID)
	{
		//// �ؽ��� ������ ä���
		//for (UINT i = 0; i < FE_TEXTURE_SIZE; i++)
		//{
		//	// texture�� �������� �ʴ´ٸ� continue
		//	if (_pTexture[i] == nullptr)
		//		continue;

		//	//// offset, tiling ������ۿ� ����
		//	//FEMaterial::_perMtrlCB.texInfo[i].ot = FEMath::FEConvertToAlignData(_texInfo[i].ot);
		//	//// angle, amount ������ۿ� ����
		//	//FEMaterial::_perMtrlCB.texInfo[i].angle_Amount = FEMath::FEConvertToAlignData(_texInfo[i].angle_Amount);
		//}

		// ���׸��� ���� ä���
		FEMaterial::_perMtrlCB.diffuse = FEMath::FEConvertToAlignData(m_diffuse);
		FEMaterial::_perMtrlCB.ambient = FEMath::FEConvertToAlignData(m_ambient);
		FEMaterial::_perMtrlCB.specular = FEMath::FEConvertToAlignData(m_specular);
		FEMaterial::_perMtrlCB.power = m_power;

		// ���̴� ������� ������Ʈ
		_pShader->UpdateConstantBufferPerMaterial();

		// ����ID ����
		_oldDrawID = GetID();
	}
}
void FEMaterial::UpdateConstantBufferPerObject(FETransform* tr)
{
	FEMaterial::_perObjCB.mPos = FEMath::FEConvertToAlignData(tr->GetPositionMatrix());
	FEMaterial::_perObjCB.mRot = FEMath::FEConvertToAlignData(tr->GetRotationMatrix());
	FEMaterial::_perObjCB.mScale = FEMath::FEConvertToAlignData(tr->GetScaleMatrix());
	FEMaterial::_perObjCB.mRP = FEMath::FEConvertToAlignData(tr->GetRotPosMatrix());
	// ���� ��� ������Ʈ
	FEMaterial::_perObjCB.mWorld = FEMath::FEConvertToAlignData(tr->GetWorldMatrix());
	tr->GetMyObject();
	// ���� * �� ��� ������Ʈ
	FEMaterial::_perObjCB.mWV = FEMaterial::_perObjCB.mWorld * FEMaterial::_perCamCB.mView;
	// ���� * �� �븻 ���  ������Ʈ
	//FEMaterial::_perObjCB.mWVNormal = FEMath::FEConvertToAlignData(tr->GetWorldMatrix().Inverse().Transpose()) * FEMaterial::_perCamCB.mView;
	// ���� * �� * �������� ��� ������Ʈ
	FEMaterial::_perObjCB.mWVP = FEMaterial::_perObjCB.mWorld * FEMaterial::_perCamCB.mView * FEMaterial::_perCamCB.mProj;
	
	// ����Ʈ ������ ������Ʈ
	auto iter = FELight::_lightList.begin();
	FEVector3 vec;

	for (UINT i = 0; iter != FELight::_lightList.end() && i < FE_LIGHT_SIZE; iter++)
	{
		if (!FEMaterial::_lightCB[i].useLight)
			continue;

		switch (FEMaterial::_lightCB[i].lightType)
		{
		case FE_LIGHT_TYPE_DIRECTION:
			FEMaterial::_perObjCB.vLightLocalDir[i] = FEMaterial::_lightCB[i].direction * FEConvertToAlignData(tr->GetRotationMatrix().Inverse());
			break;
		case FE_LIGHT_TYPE_POINT:
			FEMaterial::_perObjCB.vLightLocalPos[i] = FEMaterial::_lightCB[i].position * FEConvertToAlignData(tr->GetWorldMatrix().Inverse());
			break;
		}

		++i;
	}

	// ���̴� ������� ������Ʈ
	_pShader->UpdateConstantBufferPerObject();
}
void FEMaterial::UpdateConstantBuffer(FETransform* tr)
{
	UpdateConstantBufferPerMaterial();
	UpdateConstantBufferPerObject(tr);

	if (_constData.size() > 0)
	{
		if (_vecMatrix.size() > 0)
			memcpy_s(&_constData[0], sizeof(FEMatrixA) * _vecMatrix.size(), &_vecMatrix[0], sizeof(FEMatrixA) * _vecMatrix.size());
		if (_vecVector.size() > 0)
			memcpy_s(&_constData[_vecMatrix.size() * 4], sizeof(FEVectorA) * _vecVector.size(), &_vecVector[0], sizeof(FEVectorA) * _vecVector.size());
		if (_vecScalarA.size() > 0)
			memcpy_s(&_constData[(_vecMatrix.size() * 4) + _vecVector.size()], sizeof(FEVectorA) * _vecScalarA.size(), &_vecScalarA[0], sizeof(FEVectorA) * _vecScalarA.size());

		_pShader->UpdateConstantBuffer(&_constData[0], _constData.size() * sizeof(FEVectorA));
	}
}


void FEMaterial::Render()
{
	if (_pShader == nullptr)
		return;

	//auto sampler = FETexture::GetSampler(1);
	//
	//// ���÷� ����
	//_pShader->_pDXDC->PSSetSamplers(0, 1, &sampler);
	
	for (UINT i = 0; i < _vecTexInfo.size(); i++)
	{
		// ���̴� ���ҽ� ����.
		if (_vecTexInfo[i].pTexture == nullptr)
			//_pShader->SetShaderResources(i, _pDefaultTex);
			continue;
		else
			_pShader->SetShaderResources(i, _vecTexInfo[i].pTexture);
	}

	_pShader->Render();
}


void FEMaterial::SetShader(FEShader* i_pShader)
{
	UINT totalSize = 0;
	UINT scalarSize = 0;

	if (i_pShader == nullptr)
	{
		// Default Shader�� ����
		_pShader = FEShader::Find(L"Error");
	}

	else _pShader = i_pShader;

	scalarSize = (_pShader->_countScalar / 4) + (_pShader->_countScalar % 4 == 0 ? 0 : 1);

	totalSize += _pShader->_countMatrix * 4;
	totalSize += _pShader->_countVector;
	totalSize += scalarSize;

	_constData.resize(totalSize);
	_vecScalar.resize(scalarSize);
	_vecScalarA.resize(scalarSize);
	_vecVector.resize(_pShader->_countVector);
	_vecMatrix.resize(_pShader->_countMatrix);
	_vecTexInfo.resize(_pShader->_countTexture);
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
	if (index >= _vecTexInfo.size())	return false;

	o_texture = _vecTexInfo[index].pTexture;

	return true;
}
bool FEMaterial::SetTexture(const UINT index, FETexture* i_texture)
{
	if (index >= _vecTexInfo.size())	return false;

	_vecTexInfo[index].pTexture = i_texture;

	return true;
}
bool FEMaterial::GetTextureOffset(const UINT index, FEVector2& o_offset) const
{
	if (index >= _vecTexInfo.size())	return false;

	o_offset = FEVector2(_vecTexInfo[index].ot.x, _vecTexInfo[index].ot.y);

	return true;
}
bool FEMaterial::SetTextureOffset(const UINT index, const FEVector2 i_offset)
{
	if (index >= _vecTexInfo.size())	return false;

	_vecTexInfo[index].ot.x = i_offset.x;
	_vecTexInfo[index].ot.y = i_offset.y;

	return true;
}
bool FEMaterial::GetTextureTiling(const UINT index, FEVector2& o_tiling) const
{
	if (index >= _vecTexInfo.size())	return false;

	o_tiling = FEVector2(_vecTexInfo[index].ot.z, _vecTexInfo[index].ot.w);

	return true;
}
bool FEMaterial::SetTextureTiling(const UINT index, const FEVector2 i_tiling)
{
	if (index >= _vecTexInfo.size())	return false;

	_vecTexInfo[index].ot.z = i_tiling.x;
	_vecTexInfo[index].ot.w = i_tiling.y;

	return true;
}
bool FEMaterial::GetTextureAngle(const UINT index, FEVector3& o_angle) const
{
	if (index >= _vecTexInfo.size())	return false;

	o_angle = FEVector3(_vecTexInfo[index].angle_Amount.x, _vecTexInfo[index].angle_Amount.y, _vecTexInfo[index].angle_Amount.z);

	return true;
}
bool FEMaterial::SetTextureAngle(const UINT index, FEVector3 i_angle)
{
	if (index >= _vecTexInfo.size())	return false;

	_vecTexInfo[index].angle_Amount.x = i_angle.x;
	_vecTexInfo[index].angle_Amount.y = i_angle.y;
	_vecTexInfo[index].angle_Amount.z = i_angle.z;

	return true;
}
bool FEMaterial::GetTextureAmount(const UINT index, float& o_amount) const
{
	if (index >= _vecTexInfo.size())	return false;

	o_amount = _vecTexInfo[index].angle_Amount.w;

	return true;
}
bool FEMaterial::SetTextureAmount(const UINT index, float i_amount)
{
	if (index >= _vecTexInfo.size())	return false;

	_vecTexInfo[index].angle_Amount.w = i_amount;

	return true;
}


void FEMaterial::SetSamplerState(const UINT StartSlot, FE_SAMPLER_STATE_FLAG& sampler)
{
	_pShader->SetSamplerState(StartSlot, sampler);
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