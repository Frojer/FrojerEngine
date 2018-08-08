#include "FEObjectHeader.h"

using namespace std;

UINT IFEObject::_countID = 0;
unordered_map<UINT, IFEObject*> IFEObject::_mapObj;

IFEObject::IFEObject()
{
	_ID = _countID++;
	_mapObj[_ID] = this;
}


IFEObject::~IFEObject()
{
	_mapObj.erase(_ID);
}

UINT IFEObject::GetID()
{
	return _ID;
}

IFEObject* IFEObject::FindObject(UINT id)
{
	return _mapObj[id];
}