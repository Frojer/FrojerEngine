#pragma once
#ifndef _FE_OBJECT
#define _FE_OBJECT

#include "IFEObject.h"
#include "FEComponent.h"
#include <FEMath.h>

class FEObject : public IFEObject
{
private:
	bool _bDead;

protected:
	FEObject* _pParent;
	std::unordered_map<UINT, FEObject*> _children;
	std::unordered_map<UINT, FEComponent*> _components;

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
	FEObject(FEVector3& pos, FEVector3& rot, FEVector3& scale);
	virtual ~FEObject();

	//void Destroy();
	//void Destroy(float time);

	//void SetParent(FEObject* i_pParent);
	//FEObject* GetParent();
	//std::unordered_map<UINT, FEObject*> GetChildren();

	/*template <typename T>
	T* AddComponent()
	{
		FEComponent* pCom = nullptr;

		if (!is_base_of<FEComponent, T>::value)
			return nullptr;

		if (is_base_of<Transform, T>::value)
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
	}*/

	//FEComponent* GetComponent(const type_info& type);
	//std::unordered_map<UINT, FEComponent*> GetComponents();

	//static FEObject* Find(unsigned int id);
	//static FEObject* CopyObject(const FEObject* origin, FEVector3 pos);
	//static FEObject* CopyObject(const FEObject* origin);

	friend class IFEScene;
};

#endif