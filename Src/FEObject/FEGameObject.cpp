#include "FEObjectHeader.h"
#include <FESceneManager.h>

FEGameObject::FEGameObject()
	: _bDead(false), _pParent(nullptr)
{
	AddComponent<FETransform>();
	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
	FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
}

FEGameObject::FEGameObject(const FEVector3& pos, const FEVector3& rot, const FEVector3& scale)
	: _bDead(false), _pParent(nullptr)
{
	AddComponent<FETransform>();

	_pTransform->SetPositionLocal(pos);
	_pTransform->SetRotationRadian(rot);
	_pTransform->m_vScale = scale;

	FESceneManager::GetCurrentScene()->_mapObj[GetID()] = this;
	FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
}


FEGameObject::~FEGameObject()
{
	FESceneManager::GetCurrentScene()->_mapObj.erase(GetID());
	FESceneManager::GetCurrentScene()->_hierarchyList.remove(this);

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


void FEGameObject::Destroy()
{
	//_state |= 0x80;
	_bDead = true;
}


void FEGameObject::Destroy(float time)
{

}


FEGameObject* FEGameObject::FindChild(unsigned int id, std::unordered_map<UINT, FEGameObject*> children)
{
	FEGameObject* pObj = nullptr;

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


void FEGameObject::Initialize()
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


void FEGameObject::Update()
{
	FOR_STL(_components)
		iter->second->Update();
}


void FEGameObject::AfterUpdate()
{
	FOR_STL(_components)
		iter->second->AfterUpdate();
}


void FEGameObject::Render()
{
	FOR_STL(_components)
		iter->second->Render();

	FOR_STL(_children)
		iter->second->Render();
}


void FEGameObject::SetParent(FEGameObject* i_pParent)
{
	// 부모가 자신이거나 이미 같은 부모로 변경하려 한다면 리턴.
	if (this == i_pParent || _pParent == i_pParent)	return;

	// 부모를 가지고 있었다면, 그 부모의 자식리스트에서 제거한다.
	if (_pParent != nullptr) _pParent->_children.erase(GetID());
	// 부모를 가지고 있지 않았다면, 하이라치리스트에서 삭제한다.
	else FESceneManager::GetCurrentScene()->_hierarchyList.remove(this);

	// 부모를 변경해주고
	_pParent = i_pParent;

	// 변경한 부모가 존재하지 않는다면, 하이라치리스트에 넣는다.
	if (i_pParent == nullptr) FESceneManager::GetCurrentScene()->_hierarchyList.push_back(this);
	// 변경한 부모가 존재한다면, 그 부모의 자식으로 등록한다
	else _pParent->_children[GetID()] = this;
}


FEGameObject* FEGameObject::GetParent()
{
	return _pParent;
}


std::unordered_map<UINT, FEGameObject*> FEGameObject::GetChildren()
{
	return _children;
}


FETransform* FEGameObject::GetTransform()
{
	return _pTransform;
}


FERenderer* FEGameObject::GetRenderer()
{
	return _pRenderer;
}


std::unordered_map<UINT, FEComponent*> FEGameObject::GetComponents()
{
	return _components;
}


void FEGameObject::SetEnable(bool enable)
{
	//_state &= 0xDF;
	//_state |= (enable ? 0x20 : 0x00);
}


bool FEGameObject::GetEnable()
{
	//return ((_state & 0x40) == 0x40);
	return false;
}


FEGameObject* FEGameObject::Find(unsigned int id)
{
	if (FESceneManager::GetCurrentScene()->_mapObj.find(id) == FESceneManager::GetCurrentScene()->_mapObj.end()) return nullptr;
	return FESceneManager::GetCurrentScene()->_mapObj[id];
}


FEGameObject* FEGameObject::CopyObject(const FEGameObject* origin)
{
	FEGameObject* pObj = new FEGameObject();
	pObj->m_Name = origin->m_Name;

	FEComponent* pCom;
	IFEObject* pTemp;

	FOR_STL(origin->_components)
	{
		if (iter->second->_typeID == typeid(FETransform).hash_code())
		{
			pObj->GetTransform()->SetPositionLocal(iter->second->GetMyObject()->GetTransform()->GetPositionLocal());
			pObj->GetTransform()->SetRotationRadian(iter->second->GetMyObject()->GetTransform()->GetRotationRadian());
			pObj->GetTransform()->m_vScale = iter->second->GetMyObject()->GetTransform()->m_vScale;
		}

		else
		{
			pTemp = new IFEObject;
			pCom = (FEComponent*)new char[iter->second->_typeSize];
			memcpy_s((char*)pCom, iter->second->_typeSize, (char*)iter->second, iter->second->_typeSize);
			memcpy_s((char*)pCom + 8, sizeof(UINT), (char*)pTemp + 8, sizeof(UINT));
			delete pTemp;

			pCom->_pObj = pObj;
			pObj->_components[pCom->GetID()] = pCom;
		}
	}

	FOR_STL(origin->_children)
	{
		CopyObject(iter->second)->SetParent(pObj);
	}

	return pObj;
}