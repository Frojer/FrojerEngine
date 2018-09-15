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
	auto cams = FECamera::GetAllCameras();
	// 라이트 상수버퍼 업데이트
	FEMaterial::UpdateLightData();

	for (UINT i = 0; i < cams.size(); i++)
	{
		if (cams[i]->GetEnable())
		{
			IFERenderer::GetInstance()->SetViewports(1, &cams[i]->m_viewport);
			FEMaterial::_WVPData.mView = FEMath::FEConvertToAlignData(cams[i]->GetViewMatrixLH());
			FEMaterial::_WVPData.mProj = FEMath::FEConvertToAlignData(cams[i]->GetPerspectiveFovLH());

			auto iter = _hierarchyList.begin();

			while (iter != _hierarchyList.end())
				(*iter++)->Render();
		}
	}
}


void IFEScene::Release()
{
	auto iter = _hierarchyList.begin();

	while (iter != _hierarchyList.end())
		delete (*iter++);

	_hierarchyList.clear();
	_mapObj.clear();
}