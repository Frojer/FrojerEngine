#pragma once
#ifndef _FE_IOBJECT
#define _FE_IOBJECT

#include <FEDefine.h>
#include <unordered_map>

class IFEObject
{
private:
	INT64  _ID;

	static std::unordered_map<INT64, IFEObject*> _mapObj;

private:
	IFEObject(const IFEObject& rhs) = delete;
	IFEObject& operator= (const IFEObject& rhs) = delete;

protected:
	IFEObject(INT64 ID);

public:
	IFEObject();
	virtual ~IFEObject();

	INT64 GetID();

	static IFEObject* FindObject(INT64 id);

	friend class FELoader;
};

#endif