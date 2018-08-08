#pragma once
#ifndef _FE_OBJECT
#define _FE_OBJECT

#include <FEMath.h>
#include <typeinfo>
#include "FEObjectHeader.h"

class FEComponent;
class FETransform;
class FERenderer;

class FEObject : public IFEObject
{
private:
	bool _bDead;

protected:
	FEObject* _pParent;
	std::unordered_map<UINT, FEObject*> _children;
	std::unordered_map<UINT, FEComponent*> _components;

	FETransform* _pTransform;
	FERenderer* _pRenderer;

public:
	tstring m_Name;

private:
	FEObject(const FEObject& rhs) = delete;
	FEObject& operator= (const FEObject& rhs) = delete;

	static FEObject* FindChild(unsigned int id, std::unordered_map<UINT, FEObject*> childMap);

protected:
	void Initialize();

	void Update();
	void AfterUpdate();
	void Render();

public:
	FEObject();
	FEObject(const FEVector3& pos, const FEVector3& rot, const FEVector3& scale);
	virtual ~FEObject();

	void Destroy();
	void Destroy(float time);

	void SetParent(FEObject* i_pParent);
	FEObject* GetParent();
	std::unordered_map<UINT, FEObject*> GetChildren();

	FETransform* GetTransform();

	template <typename T>
	T* AddComponent()
	{
		FEComponent* pCom = nullptr;

		if (!std::is_base_of<FEComponent, T>::value)
			return nullptr;

		if (std::is_base_of<FETransform, T>::value)
		{
			if (_pTransform == nullptr)
			{
				pCom = new T;
				_pTransform = (FETransform*)pCom;
			}
			else
				return nullptr;
		}

		else if (std::is_base_of<FERenderer, T>::value)
		{
			if (_pRenderer == nullptr)
			{
				pCom = new T;
				_pRenderer = (FERenderer*)pCom;
			}
			else
				return nullptr;
		}

		else
		{
			pCom = new T;
		}

		pCom->_typeID = typeid(*pCom).hash_code();
		pCom->_typeSize = sizeof(T);
		pCom->_pObj = this;
		_components[pCom->GetID()] = pCom;

		return (T*)pCom;
	}

	template <typename T>
	T* GetComponent()
	{
		auto iter = _components.begin();
		while (iter != _components.end())
		{
			if (iter->second->_typeID == typeid(T).hash_code())
				return (T*)iter->second;

			iter++;
		}

		return nullptr;
	}

	std::unordered_map<UINT, FEComponent*> GetComponents();

	void SetEnable(bool enable);
	bool GetEnable();

	static FEObject* Find(unsigned int id);
	static FEObject* CopyObject(const FEObject* origin);
	//static FEObject* CopyObject(const FEObject* origin, FEVector3 pos);

	friend class IFEScene;
};

#endif