#include "FEObjectHeader.h"

using namespace std;

INT64 IFEObject::_countID = 0;
unordered_map<INT64, IFEObject*> IFEObject::_mapObj;

IFEObject::IFEObject()
{
	_ID = _countID++;
	_mapObj[_ID] = this;
}


IFEObject::~IFEObject()
{
	_mapObj.erase(_ID);
}

INT64 IFEObject::GetID()
{
	return _ID;
}

IFEObject* IFEObject::FindObject(INT64 id)
{
	if (_mapObj.find(id) == _mapObj.end()) return nullptr;
	return _mapObj[id];
}