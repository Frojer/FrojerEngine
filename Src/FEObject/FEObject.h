#pragma once
#ifndef _FE_OBJECT
#define _FE_OBJECT

#include <FEMath.h>
#include <typeinfo>
#include "FEObjectHeader.h"

class FEComponent;
class FETransform;

class FEObject : public IFEObject
{
private:
	bool _bDead;

protected:
	FEObject* _pParent;
	std::unordered_map<UINT, FEObject*> _children;
	std::unordered_map<UINT, FEComponent*> _components;

	FETransform* _pTransform;
	//Renderer* _pRenderer;

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

		/*else if (is_base_of<Renderer, T>::value)
		{
			if (m_pRenderer == nullptr)
			{
				pCom = new T;
				m_pRenderer = (Renderer*)pCom;
			}
			else
				return nullptr;
		}*/

		else
		{
			pCom = new T;
		}

		pCom->_pObj = this;
		_components[pCom->GetID()] = pCom;

		return (T*)pCom;
	}

	/*
	template <typename T>
	T* GetComponent()
	{
		FEComponent* pCom = nullptr;

		if (!is_base_of<FEComponent, T>::value)
			return nullptr;

		if (is_base_of<FETransform, T>::value)
		{
			if (m_pTransform == nullptr)
			{
				pCom = new T;
				m_pTransform = (Transform*)pCom;
			}
			else
				return nullptr;
		}

		else if (is_base_of<Renderer, T>::value)
		{
		if (m_pRenderer == nullptr)
		{
		pCom = new T;
		m_pRenderer = (Renderer*)pCom;
		}
		else
		return nullptr;
		}

		else
		{
			pCom = new T;
		}

		pCom->_pObj = this;
		_components.push_back(pCom);

		return (T*)pCom;
	}
	*/

	//FEComponent* GetComponent(const type_info& type);
	std::unordered_map<UINT, FEComponent*> GetComponents();

	void SetEnable(bool enable);
	bool GetEnable();

	static FEObject* Find(unsigned int id);
	//static FEObject* CopyObject(const FEObject* origin, FEVector3 pos);
	//static FEObject* CopyObject(const FEObject* origin);

	friend class IFEScene;
};

#endif