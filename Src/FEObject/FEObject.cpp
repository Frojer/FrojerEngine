#include "FEObject.h"
#include <FESceneManager.h>

FEObject::FEObject()
	: _bDead(false), _pParent(nullptr)
{
	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
}
FEObject::FEObject(FEVector3& pos, FEVector3& rot, FEVector3& scale)
	: _bDead(false), _pParent(nullptr)
{
	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
}


FEObject::~FEObject()
{
	FESceneManager::GetCurrentScene()->_mapObj.erase(GetID());

	auto iter = _children.begin();
	while (iter != _children.end())
	{
		delete iter->second;
		iter->second = nullptr;
		_children.erase(iter++->first);
	}

	auto iter2 = _components.begin();
	while (iter2 != _components.end())
	{
		delete iter2->second;
		iter2->second = nullptr;
		_components.erase(iter2++->first);
	}
}


FEObject* FEObject::FindChild(unsigned int id, std::unordered_map<UINT, FEObject*> children)
{
	FEObject* pObj = nullptr;

	if (children[id] != nullptr)
		return children[id];

	FOR_STL(children)
	{
		pObj = FindChild(id, iter->second->_children);

		if (pObj != nullptr)
			return pObj;
	}

	return nullptr;
}


void FEObject::Initialize()
{
	/*FOR_STL(_components)
	{
		if (CheckComponentType((*iter)->_type, COMPONENT_TYPE_UPDATE))
		{
			(*iter)->Initialize();
		}
	}

	FOR_STL(_childList)
	{
		(*iter)->Initialize();
	}*/
}


void FEObject::Update()
{

}


void FEObject::AfterUpdate()
{

}


void FEObject::Render()
{

}