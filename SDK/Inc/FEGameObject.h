#pragma once
#ifndef _FE_GAME_OBJECT
#define _FE_GAME_OBJECT

#include <FEMath.h>
#include <typeinfo>
#include "FEObjectHeader.h"
#include "FESceneManager.h"

class FEComponent;
class FETransform;
class FERenderer;

class FEGameObject : public FEObject
{
private:
	static std::unordered_map<INT64, FEGameObject*> _prefabMap;
	
	bool _bDead;
	bool _isPrefab;

protected:
	FEGameObject* _pParent;

	std::unordered_map<INT64, FEGameObject*> _children;
	std::unordered_map<INT64, FEComponent*> _components;

	FETransform* _pTransform;
	FERenderer* _pRenderer;

private:
	FEGameObject(const FEGameObject& rhs) = delete;
	FEGameObject& operator= (const FEGameObject& rhs) = delete;

	static FEGameObject* FindChild(unsigned int id, std::unordered_map<INT64, FEGameObject*> childMap);

protected:
	FEGameObject(INT64 ID);

	void Initialize();

	void Update();
	void AfterUpdate();
	void AnimationUpdate();
	void Render();

public:
	FEGameObject();
	FEGameObject(const FEVector3& pos, const FEVector3& rot, const FEVector3& scale);
	virtual ~FEGameObject();

	void Destroy();
	void Destroy(float time);

	void SetParent(FEGameObject* i_pParent);
	FEGameObject* GetParent();
	std::unordered_map<INT64, FEGameObject*> GetChildren();

	FETransform* GetTransform();
	FERenderer* GetRenderer();

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
				pCom = new FERenderer;
				if (!_isPrefab)
					FESceneManager::GetCurrentScene()->_renderMap[(static_cast<FERenderer*>(pCom))->_RenderPriority].push_back(static_cast<FERenderer*>(pCom));
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

	std::unordered_map<INT64, FEComponent*> GetComponents();

	void SetEnable(bool enable);
	bool GetEnable();

	static FEGameObject* Find(INT64 id);
	static FEGameObject* FindPrefab(INT64 id);
	static FEGameObject* CopyObject(const FEGameObject* origin);
	//static FEGameObject* CopyObject(const FEGameObject* origin, FEVector3 pos);

	friend class FELoader;
	friend class IFEScene;
};

#endif