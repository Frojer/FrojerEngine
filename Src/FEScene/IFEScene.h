#pragma once

#include <list>
#include <unordered_map>
#include <FEDefine.h>
#include <FEMath.h>
#include <FEObject.h>

enum RenderingLayer
{
	RL_1,
	RL_2,

	RL_NUM
};

class IFEScene
{
protected:
	std::unordered_map<UINT, FEObject*>	_mapObj;
	//std::map<float, FEObject*>	_mapManageDrawObj[RL_NUM];

	//std::vector<Camera*> _vecCam;

public:
	tstring m_sName;
	static FEVector4 s_backgroundColor;

public:
	IFEScene() = default;
	virtual ~IFEScene() = default;

	virtual void Load() = 0;

	void Initialize();

	void Update();
	void Render();

	void Release();

	//void AddCamera(Camera* pCam);
	//Camera* GetCamera(UINT index);
};