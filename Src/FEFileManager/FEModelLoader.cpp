#include "FEModelLoader.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <map>

using namespace std;
using namespace DirectX;

FEModelLoader* FEModelLoader::_pInstance = nullptr;

FEModelLoader::FEModelLoader()
{

}

FEModelLoader::~FEModelLoader()
{

}

FEObject* FEModelLoader::LoadModel(TCHAR* file)
{
	ASEFileParce(file);

	return currentObject;
}

FEModelLoader* FEModelLoader::GetInstance()
{
	if (_pInstance == nullptr)
		_pInstance = new FEModelLoader;

	return _pInstance;
}

void FEModelLoader::CreateModel()
{
	beforeObject = currentObject;

	currentObject = new FEObject;

	indexBeginNum = 0;
}

void FEModelLoader::SetParent(TCHAR* pParentName)
{
	if (pParentName == nullptr)
	{
		// beforeMesh룰 부모로
	}

	else
	{
		// pParentName을 부모로
	}
}

void FEModelLoader::LoadModelName(TCHAR* pName)
{
	currentObject->m_Name = pName;
}

void FEModelLoader::LoadPosition(float x, float y, float z)
{
	// object의 위치
}

void FEModelLoader::LoadRotation(float x, float y, float z)
{
	// object의 회전
}

void FEModelLoader::LoadScale(float x, float y, float z)
{
	// object의 크기
}

void FEModelLoader::LoadTexture(TCHAR* pPath)
{
	FETexture* tex = new FETexture(pPath);
}

void FEModelLoader::SetMaterial(UINT i)
{
	currentObject->AddComponent<FERenderer>()->m_pMaterial = pMtrls[i];
}

void FEModelLoader::FinishLoadModel()
{
	UINT vNum = 0;

	currentObject->GetRenderer()->m_pMesh = new FEMesh();

	bool hasColor = iColor.size() != 0;
	bool hasNormal = vVertexNormal.size() != 0;
	bool hasTex = iTex.size() != 0;

	currentObject->GetRenderer()->m_pMesh->m_verts.resize(iPos.size() * 3);
	currentObject->GetRenderer()->m_pMesh->m_indics.resize(iPos.size());

	for (UINT i = 0; i < iPos.size(); i++)
	{
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.x = vPos[iPos[i].a].x;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.y = vPos[iPos[i].a].y;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.z = vPos[iPos[i].a].z;
		if (hasColor)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.x = vColor[iColor[i].a].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.y = vColor[iColor[i].a].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.z = vColor[iColor[i].a].z;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.w = vColor[iColor[i].a].w;
		}
		if (hasNormal)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.x = vVertexNormal[vNum].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.y = vVertexNormal[vNum].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.z = vVertexNormal[vNum].z;
		}
		if (hasTex)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.x = vTex[iTex[i].a].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.y = -vTex[iTex[i].a].y;
		}

		currentObject->GetRenderer()->m_pMesh->m_indics[i].a = vNum++;

		/////////////////////////////////////////////////////////////////////////////////////

		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.x = vPos[iPos[i].b].x;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.y = vPos[iPos[i].b].y;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.z = vPos[iPos[i].b].z;
		if (hasColor)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.x = vColor[iColor[i].b].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.y = vColor[iColor[i].b].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.z = vColor[iColor[i].b].z;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.w = vColor[iColor[i].b].w;
		}
		if (hasNormal)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.x = vVertexNormal[vNum].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.y = vVertexNormal[vNum].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.z = vVertexNormal[vNum].z;
		}
		if (hasTex)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.x = vTex[iTex[i].b].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.y = -vTex[iTex[i].b].y;
		}

		currentObject->GetRenderer()->m_pMesh->m_indics[i].b = vNum++;

		/////////////////////////////////////////////////////////////////////////////////////

		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.x = vPos[iPos[i].c].x;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.y = vPos[iPos[i].c].y;
		currentObject->GetRenderer()->m_pMesh->m_verts[vNum].pos.z = vPos[iPos[i].c].z;
		if (hasColor)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.x = vColor[iColor[i].c].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.y = vColor[iColor[i].c].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.z = vColor[iColor[i].c].z;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].vtxColor.w = vColor[iColor[i].c].w;
		}
		if (hasNormal)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.x = vVertexNormal[vNum].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.y = vVertexNormal[vNum].y;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].nor.z = vVertexNormal[vNum].z;
		}
		if (hasTex)
		{
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.x = vTex[iTex[i].c].x;
			currentObject->GetRenderer()->m_pMesh->m_verts[vNum].uv.y = -vTex[iTex[i].c].y;
		}

		currentObject->GetRenderer()->m_pMesh->m_indics[i].c = vNum++;
	}

	VertexWeld(currentObject->GetRenderer()->m_pMesh);

	currentObject->GetRenderer()->m_pMesh->CreateBuffer();
}


void FEModelLoader::VertexWeld(FEMesh* pMesh)
{
	map<VF_PNT, UINT> weldMap;
	pair<map<VF_PNT, UINT>::iterator, bool> pr;

	UINT indexCount = 0;

	for (UINT i = 0; i < pMesh->m_verts.size(); i++)
	{
		pr = weldMap.insert(pair<VF_PNT, UINT>(pMesh->m_verts[i], indexCount));


		if (pr.second)	pMesh->m_verts[indexCount++] = pMesh->m_verts[i];

		switch (i % 3)
		{
		case 0:
			pMesh->m_indics[i / 3].a = pr.first->second;
			break;

		case 1:
			pMesh->m_indics[i / 3].b = pr.first->second;
			break;

		case 2:
			pMesh->m_indics[i / 3].c = pr.first->second;
			break;
		}
	}

	auto iter = pMesh->m_verts.begin();
	iter += indexCount;
	pMesh->m_verts.erase(iter, pMesh->m_verts.end());

	return;
}