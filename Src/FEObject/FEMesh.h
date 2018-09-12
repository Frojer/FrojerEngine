#pragma once
#ifndef _FE_MESH
#define _FE_MESH

#include "FEObjectHeader.h"
#include <FEMath.h>
#include <IFEBuffer.h>

enum PRIMITIVE_TOPOLOGY
{
	PRIMITIVE_TOPOLOGY_POINTLIST,
	PRIMITIVE_TOPOLOGY_LINELIST,
	PRIMITIVE_TOPOLOGY_LINESTRIP,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
};

//////////////////////////////////////////////////
//
//  정점 데이터 구조 정의
//
//////////////////////////////////////////////////
enum FE_VERTEX_FORMAT : unsigned char
{
	FE_VF_POSITION = 1,
	FE_VF_VERTEX_COLOR = 2,
	FE_VF_NORMAL = 4,
	FE_VF_TEXTURE = 8,
};

struct FE_VF_Position
{
	FEVector3 position;
};
struct FE_VF_PositionColor
{
	FEVector3 position;
	FEVector4 color;
};
struct FE_VF_PositionNormal
{
	FEVector3 position;
	FEVector3 normal;
};
struct FE_VF_PositionTexture
{
	FEVector3 position;
	FEVector2 textureCoordinate;
};
struct FE_VF_PositionColorNormal
{
	FEVector3 position;
	FEVector4 color;
	FEVector3 normal;
};
struct FE_VF_PositionColorTexture
{
	FEVector3 position;
	FEVector4 color;
	FEVector2 textureCoordinate;
};
struct FE_VF_PositionNormalTexture
{
	FEVector3 position;
	FEVector3 normal;
	FEVector2 textureCoordinate;
};
struct FE_VF_PositionColorNormalTexture
{
	FEVector3 position;
	FEVector4 color;
	FEVector3 normal;
	FEVector2 textureCoordinate;

	bool operator< (const FE_VF_PositionColorNormalTexture& rhs) const
	{
		return memcmp(this, &rhs, sizeof(FE_VF_PositionColorNormalTexture)) < 0;
	}
};

struct VF_PNT
{
	FEVector3 pos;
	FEVector3 nor;
	FEVector2 uv;
};


struct FE_IndexFormat
{
	UINT a, b, c;
};

class FEMesh : public FEObject
{
private:
	static std::unordered_map<INT64, FEMesh*> _meshMap;

protected:
	IFEBuffer* _pVB[4];
	IFEBuffer* _pIB;

public:
	std::vector<FEVector3>		m_pos;
	std::vector<FEVector4>		m_color;
	std::vector<FEVector3>		m_normal;
	std::vector<FEVector2>		m_uv;
	std::vector<FE_IndexFormat>	m_indics;

	PRIMITIVE_TOPOLOGY m_topology;

private:
	bool CreateBuffer();
	void ReleaseBuffer();

	void Render();

	static void ClearMap();

protected:
	FEMesh(INT64 ID);

public:
	FEMesh();
	virtual ~FEMesh();

	bool UpdateMeshData();

	static FEMesh* Find(INT64 id);
	static FEMesh* Find(tstring name);

	friend class FELoader;
	friend class FERenderer;
};

#endif