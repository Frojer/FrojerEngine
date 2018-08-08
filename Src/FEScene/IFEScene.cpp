#include "IFEScene.h"

FEVector4 IFEScene::s_BGColor(0.0f, 0.0f, 0.0f, 1.0f);

IFEScene::~IFEScene()
{
	Release();
}

void IFEScene::Initialize()
{
	auto iter = _mapObj.begin();
	while (iter != _mapObj.end())
		iter++->second->Initialize();
}


void IFEScene::Update()
{
	auto iter = _mapObj.begin();
	while (iter != _mapObj.end())
		iter++->second->Update();
	
	iter = _mapObj.begin();
	while (iter != _mapObj.end())
		iter++->second->AfterUpdate();
}


void IFEScene::Render()
{
	auto iter = _hierarchyList.begin();

	while (iter != _hierarchyList.end())
		(*iter++)->Render();
}


void IFEScene::Release()
{
	auto iter = _hierarchyList.begin();

	while (iter != _hierarchyList.end())
		delete (*iter++);

	_hierarchyList.clear();
	_mapObj.clear();
}