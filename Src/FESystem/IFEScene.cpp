#include "IFEScene.h"

UINT IFEScene::_maxPriority = 0;
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

	iter = _mapObj.begin();
	while (iter != _mapObj.end())
		iter++->second->AnimationUpdate();
}


void IFEScene::Render()
{
	auto cams = FECamera::GetAllCameras();
	FEMaterial::_oldDrawID = 0;

	// ����Ʈ ������� ����
	FEMaterial::UpdateConstantBufferLight();

	for (UINT i = 0; i < cams.size(); i++)
	{
		if (cams[i]->GetEnable())
		{
			// ����Ʈ ����
			IFERenderer::GetInstance()->SetViewports(1, &cams[i]->m_viewport);
			// ī�޶� ������� ����
			FEMaterial::UpdateConstantBufferPerCamera(FEMath::FEConvertToAlignData(cams[i]->GetViewMatrixLH()), FEMath::FEConvertToAlignData(cams[i]->GetPerspectiveFovLH()));

			for (UINT i = 0; i <= IFEScene::_maxPriority; i++)
			{
				auto iter = _renderMap[i].begin();
				while (iter != _renderMap[i].end())
				{
					(*iter++)->Render();
				}
			}
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