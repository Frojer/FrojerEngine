#pragma once
#ifndef _FE_MESH
#define _FE_MESH

#include "FEObjectHeader.h"
#include <FEMath.h>
#include <IFEBuffer.h>

//////////////////////////////////////////////////
//
//  정점 데이터 구조 정의
//
//////////////////////////////////////////////////
struct VF_PNT
{
	FEVector3 pos;
	FEVector3 nor;
	FEVector2 uv;
};


struct IndexFormat
{
	UINT x, y, z;
};

class FEMesh : public FEObject
{
private:
	static std::unordered_map<UINT, FEMesh*> _meshMap;

protected:
	IFEBuffer* _pVB;
	IFEBuffer* _pIB;

public:
	std::vector<VF_PNT>			m_verts;
	std::vector<IndexFormat>	m_indics;

private:
	bool CreateBuffer();

	void Render();

	static void ClearMap();

public:
	FEMesh();
	FEMesh(std::vector<VF_PNT> i_verts, std::vector<IndexFormat> i_indics);
	virtual ~FEMesh();

	static FEMesh* Find(UINT id);
	static FEMesh* Find(tstring name);
};

#endif