#include "FEObject.h"
#include <FESceneManager.h>

FEObject::FEObject()
	: _bDead(false), _pParent(nullptr)
{
	AddComponent<FETransform>();
	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
	FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
}
FEObject::FEObject(const FEVector3& pos, const FEVector3& rot, const FEVector3& scale)
	: _bDead(false), _pParent(nullptr)
{
	AddComponent<FETransform>();
	
	_pTransform->SetPositionLocal(pos);
	_pTransform->SetRotationRadian(rot);
	_pTransform->m_vScale = scale;

	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
	FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
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


void FEObject::Destroy()
{
	//_state |= 0x80;
	_bDead = true;
}


void FEObject::Destroy(float time)
{

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
	FOR_STL(_components)
		iter->second->Initialize();

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
	FOR_STL(_components)
		iter->second->Update();
}


void FEObject::AfterUpdate()
{
	FOR_STL(_components)
		iter->second->AfterUpdate();
}


void FEObject::Render()
{
	FOR_STL(_components)
		iter->second->Render();

	FOR_STL(_children)
		iter->second->Render();
}


void FEObject::SetParent(FEObject* i_pParent)
{
	if (this == i_pParent)	return;

	if (_pParent != nullptr) _pParent->_children.erase(GetID());
	else FESceneManager::GetCurrentScene()->_hierarchyList.remove(this);

	_pParent = i_pParent;

	if (i_pParent == nullptr) FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
	else _pParent->_children[GetID()] = this;
}


FEObject* FEObject::GetParent()
{
	return _pParent;
}


std::unordered_map<UINT, FEObject*> FEObject::GetChildren()
{
	return _children;
}


FETransform* FEObject::GetTransform()
{
	return _pTransform;
}


std::unordered_map<UINT, FEComponent*> FEObject::GetComponents()
{
	return _components;
}


void FEObject::SetEnable(bool enable)
{
	//_state &= 0xDF;
	//_state |= (enable ? 0x20 : 0x00);
}


bool FEObject::GetEnable()
{
	//return ((_state & 0x40) == 0x40);
	return false;
}


FEObject* FEObject::Find(unsigned int id)
{
	FEObject* pObj = nullptr;

	return FESceneManager::GetCurrentScene()->_mapObj[id];
}