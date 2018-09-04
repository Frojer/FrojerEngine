#include "FEObjectHeader.h"

#include <IFERenderer.h>

#define VERTEX_ELEMETN_NUM 4
using namespace std;

unordered_map<INT64, FEMesh*> FEMesh::_meshMap;

FEMesh::FEMesh()
	: _pVB(), _pIB(nullptr), m_topology(PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	memset(&_pVB, 0, sizeof(_pVB));
	_meshMap.insert(pair<INT64, FEMesh*>(GetID(), this));
}


FEMesh::~FEMesh()
{
	_meshMap.erase(GetID());

	ReleaseBuffer();

	m_pos.clear();
	m_color.clear();
	m_normal.clear();
	m_uv.clear();
	m_indics.clear();
}


bool FEMesh::UpdateMeshData()
{
	ReleaseBuffer();

	if (!CreateBuffer())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to update mesh data"));
		return false;
	}

	return true;
}


void FEMesh::ClearMap()
{
	auto i = _meshMap.begin();

	while (i != _meshMap.end())
		delete (i++)->second;
}


FEMesh* FEMesh::Find(INT64 id)
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
	UINT zero = 0;

	// 버텍스 버퍼 세팅
	for (UINT i = 0; i < VERTEX_ELEMETN_NUM; i++)
		pRenderer->SetVertexBuffer(i, 1, _pVB[i], _pVB[i] == nullptr ? &zero : &_pVB[i]->stride, _pVB[i] == nullptr ? &zero : &_pVB[i]->offset);
	pRenderer->SetIndexBuffer(_pIB, FEGI_FORMAT_R32_UINT, _pIB->offset);		// 인덱스 버퍼 세팅

	// 기하 위상 구조 설정 Set primitive topology
	switch (m_topology)
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
	IFEBuffer* pBuf = nullptr;
	UINT inputslot;

#define log2(x) log(x)/log(2)
	if (m_pos.size())
	{
		pBuf = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_pos.size() * sizeof(FEVector3), m_pos.data());
		if (pBuf == nullptr) return false;
		pBuf->stride = sizeof(FEVector3);
		inputslot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_POSITION));
		_pVB[inputslot] = pBuf;
	}
	if (m_color.size())
	{
		pBuf = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_color.size() * sizeof(FEVector4), m_color.data());
		if (pBuf == nullptr) return false;
		pBuf->stride = sizeof(FEVector4);
		inputslot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_COLOR));
		_pVB[inputslot] = pBuf;
	}
	if (m_normal.size())
	{
		pBuf = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_normal.size() * sizeof(FEVector3), m_normal.data());
		if (pBuf == nullptr) return false;
		pBuf->stride = sizeof(FEVector3);
		inputslot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_NORMAL));
		_pVB[inputslot] = pBuf;
	}
	if (m_uv.size())
	{
		pBuf = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DEFAULT, false, m_uv.size() * sizeof(FEVector2), m_uv.data());
		if (pBuf == nullptr) return false;
		pBuf->stride = sizeof(FEVector2);
		inputslot = (UINT)(log2((UINT)FE_SHADER_SEMANTIC_TEXCOORD));
		_pVB[inputslot] = pBuf;
	}

	_pIB = IFEBuffer::CreateBuffer(FE_BIND_INDEX_BUFFER, FE_USAGE_DEFAULT, false, m_indics.size() * sizeof(FE_IndexFormat), m_indics.data());
	if (_pIB == nullptr)	return false;

	return true;
}
void FEMesh::ReleaseBuffer()
{
	for(UINT i = 0; i < VERTEX_ELEMETN_NUM; i++)	SAFE_DELETE(_pVB[i]);
	SAFE_DELETE(_pIB);
}