#include "FEObjectHeader.h"

std::unordered_map<INT64, FETexture*> FETexture::_textureMap;

FETexture::FETexture(INT64 ID)
	: FEObject(ID), _ot(0.0f, 0.0f, 1.0f, 1.0f), _angle_Amount(0.0f, 0.0f, 0.0f, 1.0f), _pTex(nullptr)
{
	_textureMap[ID] = this;
}

FETexture::FETexture()
	: _ot(0.0f, 0.0f, 1.0f, 1.0f), _angle_Amount(0.0f, 0.0f, 0.0f, 1.0f), _pTex(nullptr)
{
	_textureMap[GetID()] = this;
}


FETexture::~FETexture()
{
	SAFE_DELETE(_pTex);
}


bool FETexture::CreateTexture(tstring file)
{
	_pTex = IFETexture::CreateTexture(file.c_str());

	if (_pTex == nullptr) return false;

	return true;
}


FEVector2 FETexture::GetOffset()
{
	return FEVector2(_ot.x, _ot.y);
}
void FETexture::SetOffset(FEVector2 offset)
{
	_ot.x = offset.x;
	_ot.y = offset.y;
}

FEVector2 FETexture::GetTiling()
{
	return FEVector2(_ot.z, _ot.w);
}
void FETexture::SetTiling(FEVector2 tiling)
{
	_ot.z = tiling.x;
	_ot.w = tiling.y;
}

FEVector3 FETexture::GetAngle()
{
	return FEVector3(_angle_Amount);
}
void FETexture::SetAngle(FEVector3 angle)
{
	_angle_Amount.x = angle.x;
	_angle_Amount.y = angle.y;
	_angle_Amount.z = angle.z;
}

float FETexture::GetAmount()
{
	return _angle_Amount.w;
}
void FETexture::SetAmount(float amount)
{
	_angle_Amount.w = amount;
}


FETexture* FETexture::Find(INT64 id)
{
	if (_textureMap.find(id) == _textureMap.end())
		return nullptr;

	return _textureMap[id];
}


FETexture* FETexture::Find(LPCTSTR name)
{
	FOR_STL(_textureMap)
	{
		if (iter->second->m_Name == name)
			return iter->second;
	}

	return nullptr;
}