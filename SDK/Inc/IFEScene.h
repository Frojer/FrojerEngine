#pragma once
#ifndef _FE_ISCENE
#define _FE_ISCENE

#include <list>
#include <unordered_map>
#include <FEDefine.h>
#include <FEMath.h>
#include "FEGameObject.h"

enum RenderingLayer
{
	RL_1,
	RL_2,

	RL_NUM
};

class IFEScene
{
protected:
	std::unordered_map<UINT, FEGameObject*>	_mapObj;
	std::list<FEGameObject*>	_hierarchyList;
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
};

#endif