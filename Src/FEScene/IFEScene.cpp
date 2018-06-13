#include "IFEScene.h"

FEVector4 IFEScene::s_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f);

void IFEScene::Initialize()
{
	auto iter = _mapObj.begin();

	while (iter != _mapObj.end())
	{
		iter++->second->Initialize();
	}
}


void IFEScene::Update()
{

}


void IFEScene::Render()
{

}


void IFEScene::Release()
{
	auto iter = _mapObj.begin();
	while (iter != _mapObj.end())
	{
		delete iter->second;
		iter->second = nullptr;
		_mapObj.erase((iter++)->first);
	}
}