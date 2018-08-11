#include "FEObjectHeader.h"

#include <IFERenderer.h>

using namespace std;

unordered_map<UINT, FEMesh*> FEMesh::_meshMap;

FEMesh::FEMesh()
	: _pVB(nullptr), _pIB(nullptr)
{
	_meshMap.insert(pair<UINT, FEMesh*>(GetID(), this));
}

FEMesh::FEMesh(vector<VF_PNT> i_verts, vector<IndexFormat> i_indics)
	: _pVB(nullptr), _pIB(nullptr), m_verts(i_verts), m_indics(i_indics)
{
	_meshMap.insert(pair<UINT, FEMesh*>(GetID(), this));
}


FEMesh::~FEMesh()
{
	_meshMap.erase(GetID());

	SAFE_RELEASE(_pVB);
	SAFE_RELEASE(_pIB);

	m_verts.clear();
	m_indics.clear();
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


void FEMesh::Render()
{
	IFERenderer* pRenderer = IFERenderer::GetInstance();

	UINT stride = sizeof(VF_PNT);
	UINT offset = 0;
	pRenderer->SetVertexBuffers(0, 1, _pVB, &stride, &offset); // 버텍스 버퍼 세팅
	pRenderer->SetIndexBuffer(_pIB, FEGI_FORMAT_R32_UINT, offset);		// 인덱스 버퍼 세팅

	// 기하 위상 구조 설정 Set primitive topology
	pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool FEMesh::CreateBuffer()
{
	_pVB = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_verts.size() * sizeof(VF_PNT), m_verts.data());
	if (_pVB == nullptr)	return false;

	_pIB = IFEBuffer::CreateBuffer(FE_BIND_INDEX_BUFFER, FE_USAGE_DEFAULT, false, m_indics.size() * sizeof(IndexFormat), m_indics.data());
	if (_pIB == nullptr)	return false;

	return true;
}