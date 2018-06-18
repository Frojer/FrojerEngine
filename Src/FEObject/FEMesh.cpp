#include "FEObjectHeader.h"

#include <FESystem.h>

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
	if (_meshMap.find(id) == _meshMap.end())
		return nullptr;

	return _meshMap[id];
}


FEMesh* FEMesh::Find(wstring name)
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
	auto pRenderer = FESystem::GetInstance()->GetRenderer();

	//----------------------------------------------------------------------
	// 바른 렌더링 결과를 위해서는 아래의 조건이 동일 또는 호환되어야 합니다.★
	// 1.정점 버퍼의 데이터.  Vertex Buffer Data
	// 2.정점 구조 Vertex Format (Input Layout)
	// 3.셰이더 함수의 입력구조.  Vertex Shader (Input Layout)
	// 4.각종 변환 처리 Vertex Transform
	//----------------------------------------------------------------------
	//정점 버퍼 설정. Set vertex buffer
	//디바이스의 입력-조립 스테이지 (Input-Assembler Stage) 에 정점버퍼를 연결.(Binding)
	UINT stride = sizeof(VF_PNT);
	UINT offset = 0;
	pRenderer->SetVertexBuffers(0, 1, &_pVB, &stride, &offset); // 버텍스 버퍼 세팅
	pRenderer->SetIndexBuffer(_pIB, FEGI_FORMAT_R32_UINT, offset);		// 인덱스 버퍼 세팅

	// 기하 위상 구조 설정 Set primitive topology
	pRenderer->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool FEMesh::CreateBuffer(vector<VF_PNT>& i_vertics, vector<IndexFormat>& i_indics)
{
	m_verts = i_vertics;
	m_indics = i_indics;

	auto pRenderer = FESystem::GetInstance()->GetRenderer();

	_pVB = pRenderer->CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_verts.size() * sizeof(VF_PNT), m_verts.data());
	if (_pVB == nullptr)
		return false;

	_pIB = pRenderer->CreateBuffer(FE_BIND_INDEX_BUFFER, FE_USAGE_DEFAULT, false, m_indics.size() * sizeof(IndexFormat), m_indics.data());
	if (_pIB == nullptr)
		return false;

	return true;
}