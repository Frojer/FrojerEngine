#pragma once
#ifndef _FE_IOBJECT
#define _FE_IOBJECT

#include <FEDefine.h>
#include <unordered_map>

class IFEObject
{
private:
	static UINT _countID;
	UINT  _ID;

	static std::unordered_map<UINT, IFEObject*> _mapObj;

public:
	tstring m_Name;
	IFEObject(const IFEObject& rhs) = delete;

public:
	IFEObject();
	virtual ~IFEObject();

	UINT GetID();

	static IFEObject* FindObject(UINT id);
};

#endif