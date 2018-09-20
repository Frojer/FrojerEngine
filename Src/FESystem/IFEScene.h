#pragma once
#ifndef _FE_ISCENE
#define _FE_ISCENE

#include <list>
#include <unordered_map>
#include <FEDefine.h>
#include <FEMath.h>
#include "FEGameObject.h"
class FEGameObject;

class IFEScene
{
protected:
	static UINT _maxPriority;
	std::unordered_map<INT64, FEGameObject*>	_mapObj;
	std::list<FEGameObject*>	_hierarchyList;
	std::unordered_map<UINT, std::list<FERenderer*> > _renderMap;
	//std::map<float, FEGameObject*>	_mapManageDrawObj[RL_NUM];

public:
	tstring m_sName;
	static FEVector4 s_BGColor;

public:
	IFEScene() = default;
	virtual ~IFEScene();

	virtual void Load() = 0;

	void Initialize();

	void Update();
	void Render();

	void Release();

	friend class FEGameObject;
	friend class FERenderer;
};

#endif