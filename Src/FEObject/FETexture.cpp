#include "FEObjectHeader.h"

std::unordered_map<INT64, FETexture*> FETexture::_textureMap;

FETexture::FETexture(INT64 ID)
	: FEObject(ID), _pTex(nullptr)
{
	_textureMap[ID] = this;
}

FETexture::FETexture()
	: _pTex(nullptr)
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