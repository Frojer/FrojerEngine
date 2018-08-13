#include "FEObjectHeader.h"

#include <IFERenderer.h>

using namespace std;

unordered_map<UINT, FEMesh*> FEMesh::_meshMap;

FEMesh::FEMesh(vector<VF_PNT> i_verts, vector<IndexFormat> i_indics)
	: _pVB(nullptr), _pIB(nullptr), m_verts(i_verts), m_indics(i_indics)
{
	_meshMap.insert(pair<UINT, FEMesh*>(GetID(), this));
}


FEMesh::~FEMesh()
{
	_meshMap.erase(GetID());

	SAFE_DELETE(_pVB);
	SAFE_DELETE(_pIB);

	m_verts.clear();
	m_indics.clear();
}


FEMesh* FEMesh::CreateMesh(std::vector<VF_PNT> i_verts, std::vector<IndexFormat> i_indics)
{
	FEMesh* pMesh = new FEMesh(i_verts, i_indics);

	if (pMesh == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to dynamic allocate"));
		return nullptr;
	}

	if (!pMesh->CreateBuffer())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to create mesh"));
		delete pMesh;
		return nullptr;
	}

	return pMesh;
}


void FEMesh::ClearMap()
{
	auto i = _meshMap.begin();

	while (i != _meshMap.end())
		delete (i++)->second;
}


FEMesh* FEMesh::Find(UINT id)
{
	if (_meshMap.find(id) == _meshMap.end())	return nullptr;
	return _meshMap[id];
}


FEMesh* FEMesh::Find(tstring name)
{
	FOR_STL(_meshMap)
	{
		if (iter->second->m_Name == name)
			return iter->second;
	}

	return nullptr;
}


void FEMesh::Render(PRIMITIVE_TOPOLOGY topology)
{
	IFERenderer* pRenderer = IFERenderer::GetInstance();

	UINT stride = sizeof(VF_PNT);
	UINT offset = 0;
	pRenderer->SetVertexBuffers(0, 1, _pVB, &stride, &offset); // 버텍스 버퍼 세팅
	pRenderer->SetIndexBuffer(_pIB, FEGI_FORMAT_R32_UINT, offset);		// 인덱스 버퍼 세팅

	// 기하 위상 구조 설정 Set primitive topology
	switch (topology)
	{
	case PRIMITIVE_TOPOLOGY_POINTLIST:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;

	case PRIMITIVE_TOPOLOGY_LINELIST:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_LINELIST);
		break;

	case PRIMITIVE_TOPOLOGY_LINESTRIP:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_LINESTRIP);
		break;

	case PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;

	case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;

	case PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
		break;

	case PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ);
		break;

	case PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
		break;

	case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ);
		break;
	}
}


bool FEMesh::CreateBuffer()
{
	_pVB = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_verts.size() * sizeof(VF_PNT), m_verts.data());
	if (_pVB == nullptr)	return false;

	_pIB = IFEBuffer::CreateBuffer(FE_BIND_INDEX_BUFFER, FE_USAGE_DEFAULT, false, m_indics.size() * sizeof(IndexFormat), m_indics.data());
	if (_pIB == nullptr)	return false;

	return true;
}