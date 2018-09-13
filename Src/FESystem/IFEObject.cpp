#include "FEObjectHeader.h"
#include <FEUtility.h>

using namespace std;

unordered_map<INT64, IFEObject*> IFEObject::_mapObj;

IFEObject::IFEObject(INT64 ID)
{
	_ID = ID;
	_mapObj.insert(pair<INT64, IFEObject*>(ID, this));
}

IFEObject::IFEObject()
{
	_ID = CreateUUIDHashCode64();
	while (!_mapObj.insert(pair<INT64, IFEObject*>(_ID, this)).second) _ID = CreateUUIDHashCode64();
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