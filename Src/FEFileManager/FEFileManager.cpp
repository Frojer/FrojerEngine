#include "FEFileManager.h"

#include <map>

#include <FEUtility.h>

//-------------------------------
// 폴더 내 파일 찾기
//-------------------------------
#include <io.h>
#include <conio.h>

#define BUFFER_SIZE 1024

FEFileManager* FEFileManager::_pInstance = nullptr;

FEFileManager::FEFileManager()
{

}
FEFileManager::~FEFileManager()
{

}

struct ASE_ANIM_POS
{
	float animTime;
	FEVector3 pos;
};
struct ASE_ANIM_ROT
{
	float animTime;
	FEVector4 qRot = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
};
struct ASE_ANIM_SCALE
{
	float animTime;
	FEVector3 scale;
	FEVector4 qScaleRot = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
};

struct ASE_MESH
{
	INT64 ID;
	tstring name;
	tstring parentName;

	INT64 mtrlID;

	std::list<ASE_MESH*> children;

	FEVector3 vPosWorld;
	FEVector4 qRotWorld;
	FEVector3 vScaleWorld;
	FEVector4 qScaleRotWorld;

	FEVector3 vPos;
	FEVector4 qRot;
	FEVector3 vScale;
	FEVector4 qScaleRot;

	FEMatrix TM;

	unsigned char vf;

	std::vector<FEVector3>		position;
	std::vector<FE_IndexFormat>	index;
	std::vector<FEVector4>		color;
	std::vector<FEVector3>		normal;
	std::vector<FEVector2>		texcoord;

	// 애니메이션
	bool useAnim;
	std::vector<ASE_ANIM_POS> animPos;
	std::vector<ASE_ANIM_ROT> animRot;
	std::vector<ASE_ANIM_SCALE> animScale;

	ASE_MESH()
		: ID(0)
		, mtrlID(0)
		, qRotWorld(0.0f, 0.0f, 0.0f, 1.0f)
		, vScaleWorld(1.0f, 1.0f, 1.0f)
		, qScaleRotWorld(0.0f, 0.0f, 0.0f, 1.0f)
		, qRot(0.0f, 0.0f, 0.0f, 1.0f)
		, vScale(1.0f, 1.0f, 1.0f)
		, qScaleRot(0.0f, 0.0f, 0.0f, 1.0f)
		, vf(0)
		, TM(FEMatrix::Identity)
		, useAnim(false)
	{
	}
};

void EscapeBlock(tifstream& f)
{
	TCHAR buf[BUFFER_SIZE];

	while (true)
	{
		f >> buf;

		if (buf[0] == FE_TEXT('}'))
			return;

		else if (buf[0] == FE_TEXT('{'))
			EscapeBlock(f);
	}
}
INT64 ConvertTextureFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	INT64 uuid = 0;
	tifstream mif(i_filePath + i_fileName, std::ios::binary);
	tofstream mof(i_outPath + i_fileName, std::ios::binary);

	if (mif.is_open() && mof.is_open())
	{
		FECopyFile(mif, mof);

		mof.close();

		mof.open(i_outPath + i_fileName + FE_TEXT(".fet"));
		uuid = CreateUUIDHashCode64();
		mof << FE_TEXT("ID = ") << uuid;
	}

	return uuid;
}
void ConvertShaderFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{

}
void CompileVertexShader(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tstring command = FE_TEXT("fxc.exe");
	command.append(FE_TEXT(" /E VS_Main"));
	command.append(FE_TEXT(" /T vs_5_0"));
	command.append(FE_TEXT(" /Fo ") + i_outPath + GetFileName(i_fileName) + FE_TEXT(".vso"));
	command.append(FE_TEXT(" ") + i_filePath + i_fileName);
	// 열기준 연산 행기준은 /Zpc
	command.append(FE_TEXT(" /Zpr"));

	_tsystem(command.c_str());
}
void CompilePixelShader(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tstring command = FE_TEXT("fxc.exe");
	command.append(FE_TEXT(" /E PS_Main"));
	command.append(FE_TEXT(" /T ps_5_0"));
	command.append(FE_TEXT(" /Fo ") + i_outPath + GetFileName(i_fileName) + FE_TEXT(".pso"));
	command.append(FE_TEXT(" ") + i_filePath + i_fileName);
	// 열기준 연산 행기준은 /Zpc
	command.append(FE_TEXT(" /Zpr"));

	_tsystem(command.c_str());
}
void WriteMeshData(tofstream& o, ASE_MESH* mesh, UINT depth)
{
#define TAP for (UINT di = 0; di < depth; di++) o << FE_TEXT('\t');
	UINT i = 0;

	TAP	o << FE_TEXT("MeshObject") << std::endl;
	TAP	o << FE_TEXT("{") << std::endl;
	depth++;
	// 오프젝트 ID
	TAP	o << FE_TEXT("ObjectID = ") << CreateUUIDHashCode64() << std::endl;

	// 메쉬 ID
	TAP	o << FE_TEXT("MeshID = ") << mesh->ID << std::endl;
	// 메쉬 이름 쓰기
	TAP	o << FE_TEXT("Name =") << mesh->name << std::endl;

	// 머테리얼 ID 쓰기
	if (mesh->mtrlID != 0)	{ TAP	o << FE_TEXT("MaterialID = ") << mesh->mtrlID << std::endl;	}
	else					{ TAP	o << FE_TEXT("MaterialID = ") << 0 << std::endl; }

	// 트렌스폼 쓰기
	FEVector4 Q;
	TAP	o << FE_TEXT("Pos = ") << mesh->vPos.x << FE_TEXT('\t') << mesh->vPos.y << FE_TEXT('\t') << mesh->vPos.z << std::endl;
	TAP	o << FE_TEXT("QRot = ") << mesh->qRot.x << FE_TEXT('\t') << mesh->qRot.y << FE_TEXT('\t') << mesh->qRot.z << FE_TEXT('\t') << mesh->qRot.w << std::endl;
	TAP	o << FE_TEXT("Scale = ") << mesh->vScale.x << FE_TEXT('\t') << mesh->vScale.y << FE_TEXT('\t') << mesh->vScale.z << std::endl;
	TAP	o << FE_TEXT("QScaleRot = ") << mesh->qScaleRot.x << FE_TEXT('\t') << mesh->qScaleRot.y << FE_TEXT('\t') << mesh->qScaleRot.z << FE_TEXT('\t') << mesh->qScaleRot.w << std::endl;

	// 버텍스 포맷 쓰기
	TAP	o << FE_TEXT("VertexFormat = ") << mesh->vf << std::endl;

	// 버텍스 정보들 쓰기
	TAP	o << FE_TEXT("VertexCount = ") << mesh->position.size() << std::endl;

	for (i = 0; i < mesh->position.size(); i++)
	{
		if ((mesh->vf & FE_VF_POSITION) == FE_VF_POSITION)			{ TAP	o << mesh->position[i].x << FE_TEXT('\t') << mesh->position[i].y << FE_TEXT('\t') << mesh->position[i].z << std::endl; }
		if ((mesh->vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)	{ TAP	o << mesh->color[i].x << FE_TEXT('\t') << mesh->color[i].y << FE_TEXT('\t') << mesh->color[i].z << FE_TEXT('\t') << mesh->color[i].w << std::endl; }
		if ((mesh->vf & FE_VF_NORMAL) == FE_VF_NORMAL)				{ TAP	o << mesh->normal[i].x << FE_TEXT('\t') << mesh->normal[i].y << FE_TEXT('\t') << mesh->normal[i].z << std::endl; }
		if ((mesh->vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)			{ TAP	o << mesh->texcoord[i].x << FE_TEXT('\t') << mesh->texcoord[i].y << std::endl; }
	}

	// 인덱스 정보들 쓰기
	TAP	o << FE_TEXT("IndexCount = ") << mesh->index.size() << std::endl;

	for (i = 0; i < mesh->index.size(); i++)
	{
		TAP	o << mesh->index[i].a << FE_TEXT('\t') << mesh->index[i].b << FE_TEXT('\t') << mesh->index[i].c << std::endl;
	}

	TAP	o << FE_TEXT("UseAnim = ") << mesh->useAnim << std::endl;
	if (mesh->useAnim)
	{
		// 애니메이션 정보 쓰기
		TAP	o << FE_TEXT("AnimPos = ") << mesh->animPos.size() << std::endl;
		for (i = 0; i < mesh->animPos.size(); i++)
		{
			TAP o << mesh->animPos[i].animTime << FE_TEXT('\t') << mesh->animPos[i].pos.x << FE_TEXT('\t') << mesh->animPos[i].pos.y << FE_TEXT('\t') << mesh->animPos[i].pos.z << std::endl;
		}
		TAP o << FE_TEXT("AnimRot = ") << mesh->animRot.size() << std::endl;
		for (int i = 0; i < mesh->animRot.size(); i++)
		{
			TAP o << mesh->animRot[i].animTime << FE_TEXT('\t') << mesh->animRot[i].qRot.x << FE_TEXT('\t') << mesh->animRot[i].qRot.y << FE_TEXT('\t') << mesh->animRot[i].qRot.z << FE_TEXT('\t') << mesh->animRot[i].qRot.w << std::endl;
		}
		TAP o << FE_TEXT("AnimScale = ") << mesh->animScale.size() << std::endl;
		for (int i = 0; i < mesh->animScale.size(); i++)
		{
			TAP o << mesh->animScale[i].animTime << FE_TEXT('\t') << mesh->animScale[i].scale.x << FE_TEXT('\t') << mesh->animScale[i].scale.y << FE_TEXT('\t') << mesh->animScale[i].scale.z << FE_TEXT('\t') << mesh->animScale[i].qScaleRot.x << FE_TEXT('\t') << mesh->animScale[i].qScaleRot.y << FE_TEXT('\t') << mesh->animScale[i].qScaleRot.z << FE_TEXT('\t') << mesh->animScale[i].qScaleRot.w << std::endl;
		}
	}

	for (auto iter = mesh->children.begin(); iter != mesh->children.end(); iter++)
	{
		WriteMeshData(o, (*iter), depth);
	}

	depth--;
	TAP	o << FE_TEXT("}") << std::endl;

#undef TAP
}
void VertexWeld(std::vector<FE_VF_PositionColorNormalTexture>& verts, std::vector<FE_IndexFormat>& indics)
{
	std::map<FE_VF_PositionColorNormalTexture, UINT> weldMap;
	std::pair<std::map<FE_VF_PositionColorNormalTexture, UINT>::iterator, bool> pr;

	UINT indexCount = 0;

	for (UINT i = 0; i < verts.size(); i++)
	{
		pr = weldMap.insert(std::pair<FE_VF_PositionColorNormalTexture, UINT>(verts[i], indexCount));


		if (pr.second)	verts[indexCount++] = verts[i];

		switch (i % 3)
		{
		case 0:
			indics[i / 3].a = pr.first->second;
			break;

		case 1:
			indics[i / 3].b = pr.first->second;
			break;

		case 2:
			indics[i / 3].c = pr.first->second;
			break;
		}
	}

	auto iter = verts.begin();
	iter += indexCount;
	verts.erase(iter, verts.end());

	return;
}
//void WriteASEAnim(tofstream& o, ASE_MESH* mesh, UINT depth)
//{
//#define TAP for (UINT di = 0; di < depth; di++) o << FE_TEXT('\t');
//	TAP o << FE_TEXT("Animation") << std::endl;
//	TAP o << FE_TEXT("{") << std::endl;
//
//	depth++;
//
//	TAP o << FE_TEXT("Name = ") << mesh->name << std::endl;
//	TAP o << FE_TEXT("Pos") << FE_TEXT('\t') << mesh->animPos.size() << std::endl;
//	for (int i = 0; i < mesh->animPos.size(); i++)
//	{
//		TAP o << mesh->animPos[i].animTime << FE_TEXT('\t') << mesh->animPos[i].pos.x << FE_TEXT('\t') << mesh->animPos[i].pos.y << FE_TEXT('\t') << mesh->animPos[i].pos.z << std::endl;
//	}
//	TAP o << FE_TEXT("Rot") << FE_TEXT('\t') << mesh->animRot.size() << std::endl;
//	for (int i = 0; i < mesh->animRot.size(); i++)
//	{
//		TAP o << mesh->animRot[i].animTime << FE_TEXT('\t') << mesh->animRot[i].axis.x << FE_TEXT('\t') << mesh->animRot[i].axis.y << FE_TEXT('\t') << mesh->animRot[i].axis.z << FE_TEXT('\t') << mesh->animRot[i].angle << std::endl;
//	}
//	TAP o << FE_TEXT("Scale") << FE_TEXT('\t') << mesh->animScale.size() << std::endl;
//	for (int i = 0; i < mesh->animScale.size(); i++)
//	{
//		TAP o << mesh->animScale[i].animTime << FE_TEXT('\t') << mesh->animScale[i].scale.x << FE_TEXT('\t') << mesh->animScale[i].scale.y << FE_TEXT('\t') << mesh->animScale[i].scale.z << FE_TEXT('\t') << mesh->animScale[i].axis.x << FE_TEXT('\t') << mesh->animScale[i].axis.y << FE_TEXT('\t') << mesh->animScale[i].axis.z << FE_TEXT('\t') << mesh->animScale[i].angle << std::endl;
//	}
//
//	for (auto iter = mesh->children.begin(); iter != mesh->children.end(); iter++)
//	{
//		WriteASEAnim(o, (*iter), depth);
//	}
//
//	depth--;
//	o << FE_TEXT("}") << std::endl;
//#undef TAP
//}
void ConvertASEMapFile(tifstream& f, tofstream& o, tstring i_filePath, tstring i_outPath, TCHAR buf[])
{
	int a, b, c, i = 0;
	float x, y, z, w;

	INT64 uuid = 0;

	TCHAR mapClass[BUFFER_SIZE];

	o << FE_TEXT('\t') << FE_TEXT("{") << std::endl;

	f >> buf;
	// Map Name
	f >> buf;
	f.getline(buf, BUFFER_SIZE);
	// Map Class
#pragma region MAP_CLASS
	f >> buf;
	i = 0;

	f.getline(buf, BUFFER_SIZE);

	while (buf[i] != FE_TEXT('"'))	i++;
	int s = ++i;
	while (buf[i] != FE_TEXT('"'))	i++;
	int e = i;

	_tcsncpy_s(mapClass, buf + s, e - s);
#pragma endregion
	// Map subno
	f >> buf >> i;
	// Map Amount
	f >> buf >> x;
	o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("Amount = ") << x << std::endl;

	// 머테리얼 클래스에 따른 처리
	if (TCSCMP_SAME(mapClass, FE_TEXT("Bitmap")))
	{
		// BITMAP "경로" : 경로 추출;
		f >> buf;
		i = 0;

		f.getline(buf, BUFFER_SIZE);

		while (buf[i] != FE_TEXT('"'))	i++;
		int s = ++i;
		while (buf[i] != FE_TEXT('"'))	i++;
		int e = i;

		_tcsncpy_s(mapClass, buf + s, e - s);

		if (_taccess((i_outPath + GetFileNameWithExtension(mapClass)).c_str(), 0) == -1)
		{
			uuid = ConvertTextureFile(i_filePath, i_outPath, GetFileNameWithExtension(mapClass));
		}
		else
		{
			tifstream tf(i_outPath + GetFileNameWithExtension(mapClass) + FE_TEXT(".fet"));

			if (tf.is_open())
			{
				tf >> buf >> buf >> uuid;
			}
		}

		o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("MapID = ") << uuid << std::endl;
		
		// MAP_TYPE Screen;
		f >> buf >> buf;
		// UVW_U_OFFSET 0.0000;
		f >> buf >> x;
		// UVW_V_OFFSET 0.0000;
		f >> buf >> y;
		o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("Offset") << FE_TEXT('\t') << x << FE_TEXT('\t') << y << std::endl;
		// UVW_U_TILING 1.0000;
		f >> buf >> x;
		// UVW_V_TILING 1.0000;
		f >> buf >> y;
		o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("Tiling") << FE_TEXT('\t') << x << FE_TEXT('\t') << y << std::endl;
		// UVW_ANGLE 0.0000;
		f >> buf >> x;
		o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("Angle") << FE_TEXT('\t') << x << std::endl;
		// UVW_BLUR 1.0000;
		f >> buf >> x;
		// UVW_BLUR_OFFSET 0.0000;
		f >> buf >> x;
		// UVW_NOUSE_AMT 1.0000;;
		f >> buf >> x;
		// UVW_NOISE_SIZE 1.0000;
		f >> buf >> x;
		// UVW_NOISE_LEVEL 1;
		f >> buf >> i;
		// UVW_NOISE_PHASE 0.0000;
		f >> buf >> x;
		// BITMAP_FILTER
		f >> buf >> buf;
		// '}'
		f >> buf;
	}
	else
	{
		while (true)
		{
			f >> buf;

			if (buf[0] == FE_TEXT('}'))
				break;

			else if (buf[0] == FE_TEXT('{'))
				EscapeBlock(f);
		}
	}
	o << FE_TEXT('\t') << FE_TEXT("}") << std::endl;
}
void ConvertASEMaterialFile(tifstream& f, tofstream& o, tstring i_filePath, tstring i_outPath, TCHAR buf[], std::vector<INT64>& vMtrl)
{
	tstringstream ss;

	tstring mtrlClass;

	int a, b, c, i = 0;
	float x, y, z, w;

	FEVector4 ambient;
	FEVector4 diffuse;
	FEVector4 specular;
	float shine, shineStrength, transparency;

	o << FE_TEXT("Material") << std::endl;
	o << FE_TEXT("{") << std::endl;

	// UUID
	f >> i >> buf;
	vMtrl[i] = CreateUUIDHashCode64();
	o << FE_TEXT('\t') << FE_TEXT("ID = ") << vMtrl[i] << std::endl;

	// MATERIAL_NAME
	f >> buf;
	f.getline(buf, BUFFER_SIZE);
	o << FE_TEXT('\t') << FE_TEXT("Name =") << buf << std::endl;

	// MATERIAL_CLASS
	f >> buf;
	f.getline(buf, BUFFER_SIZE);
	mtrlClass = StripQuotes(buf);

	f >> buf >> ambient.x >> ambient.y >> ambient.z;
	f >> buf >> diffuse.x >> diffuse.y >> diffuse.z;
	f >> buf >> specular.x >> specular.y >> specular.z;
	f >> buf >> shine;
	f >> buf >> shineStrength;
	f >> buf >> transparency;
	// MATERIAL_WIRESIZE
	f >> buf >> x;

	// 머테리얼 정보 출력
	o << FE_TEXT('\t') << FE_TEXT("Diffuse") << FE_TEXT('\t') << diffuse.x << FE_TEXT('\t') << diffuse.y << FE_TEXT('\t') << diffuse.z << FE_TEXT('\t') << 1.0f - transparency << std::endl;
	o << FE_TEXT('\t') << FE_TEXT("Ambient") << FE_TEXT('\t') << ambient.x << FE_TEXT('\t') << ambient.y << FE_TEXT('\t') << ambient.z << std::endl;
	o << FE_TEXT('\t') << FE_TEXT("Specular") << FE_TEXT('\t') << specular.x << FE_TEXT('\t') << specular.y << FE_TEXT('\t') << specular.z << std::endl;
	o << FE_TEXT('\t') << FE_TEXT("Power = ") << shine * 1000 * shineStrength << std::endl;

	// 머테리얼 클래스에 따른 처리
	//if (TCSCMP_SAME(mtrlClass.c_str(), FE_TEXT("Standard")))
	if (mtrlClass == FE_TEXT("Standard"))
	{
		// MATERIAL_SHADING
		f >> buf >> buf;
		// MATERIAL_XP_FALLOFF
		f >> buf >> x;
		// MATERIAL_SELFILLUM
		f >> buf >> x;
		// MATERIAL_FALLOFF
		f >> buf >> buf;
		// MATERIAL_XP_TYPE
		f >> buf >> buf;

		while (true)
		{
			f >> buf;

			if (buf[0] == FE_TEXT('}'))
				break;

			else if (buf[0] == FE_TEXT('{'))
				EscapeBlock(f);

			else if (buf[0] != FE_TEXT('*'))
			{
				f.getline(buf, BUFFER_SIZE);
				continue;
			}
			
			if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_DIFFUSE")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 0 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SPECULAR")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 1 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SHINE")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 2 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SHINESTRENGTH")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 3 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SELFILLUM")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 4 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_OPACITY")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 5 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_FILTERCOLOR")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 6 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_BUMP")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 7 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_REFLECT")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 8 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_REFRACT")))
			{
				o << FE_TEXT('\t') << FE_TEXT("Map ") << 9 << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
		}
	}
	else
	{
		while (true)
		{
			f >> buf;

			if (buf[0] == FE_TEXT('}'))
				break;

			else if (buf[0] == FE_TEXT('{'))
				EscapeBlock(f);
		}
	}

	o << FE_TEXT("}") << std::endl;
}
bool ConvertASEMeshFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tostringstream ss;
	tofstream o;
	tifstream f;

	TCHAR buf[BUFFER_SIZE];

	// 애니메이션은 총 몇프레임으로 되어있는가?
	int lastFrame;
	// 초당 몇 프레임인가?
	int frameSpeed;
	// 프레임당 몇 틱인가? (ASE Animation 파일 내의 TICK값)
	int ticksPerFrame;

	std::vector<INT64> vMtrl;
	std::list<ASE_MESH*> meshList;

	std::vector<FEVector3>		vPos;
	std::vector<FE_IndexFormat>	iPos;
	std::vector<FEVector4>		vColor;
	std::vector<FE_IndexFormat>	iColor;
	std::vector<FEVector3>		vVertexNormal;
	std::vector<FEVector3>		vFaceNormal;
	std::vector<FEVector2>		vTex;
	std::vector<FE_IndexFormat>	iTex;

	FEVector3 pos, rot, scale;
	FEVector3 scaleAxis;
	float rotAngle, scaleAxisAngle;

	int a, b, c, i = 0;
	float x, y, z, w;

	// 원본 파일과 저장할 파일에 대한 객체 생성
	o.open(i_outPath + GetFileName(i_fileName) + FE_TEXT(".msh"));
	if (!o.is_open())	return false;
	f.open(i_filePath + i_fileName);
	if (!f.is_open())	return false;

	// ASE 파싱
	while (!f.eof())
	{
		f >> buf;

		// 커맨드인가?
		if (buf[0] != FE_TEXT('*'))
		{
			f.getline(buf, BUFFER_SIZE);
			continue;
		}

		if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE")))
		{
			f >> buf;

			while (true)
			{
				f >> buf;

				if (buf[0] == FE_TEXT('}'))			break;
				else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
				else if (buf[0] != FE_TEXT('*'))	continue;

				if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FILENAME")))
				{
					f.getline(buf, BUFFER_SIZE);
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FIRSTFRAME")))
				{
					f >> a;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_LASTFRAME")))
				{
					f >> lastFrame;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FRAMESPEED")))
				{
					f >> frameSpeed;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_TICKSPERFRAME")))
				{
					f >> ticksPerFrame;
					o << FE_TEXT("AnimationTime = ") << (float)lastFrame / (float)frameSpeed << std::endl;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_BACKGROUND_STATIC")))
				{
					f >> x >> y >> z;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_AMBIENT_STATIC")))
				{
					f >> x >> y >> z;
				}
			}
		}
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_LIST")))
		{
			f >> buf;

			while (true)
			{
				f >> buf;

				if (buf[0] == FE_TEXT('}'))			break;
				else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
				else if (buf[0] != FE_TEXT('*'))	continue;

				if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_COUNT")))
				{
					f >> i;

					vMtrl.resize(i);
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL")))
				{
					ConvertASEMaterialFile(f, o, i_filePath, i_outPath, buf, vMtrl);
				}
			}
		}
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("GEOMOBJECT")))
		{
			ASE_MESH* mesh = new ASE_MESH;
			FEMatrix m = FEMatrix::Identity;

			vPos.clear();
			iPos.clear();
			vColor.clear();
			iColor.clear();
			vVertexNormal.clear();
			vFaceNormal.clear();
			vTex.clear();
			iTex.clear();

			mesh->ID = CreateUUIDHashCode64();

			f >> buf;

			while (true)
			{
				f >> buf;

				if (buf[0] == FE_TEXT('}'))			break;
				else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
				else if (buf[0] != FE_TEXT('*'))	continue;

				if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
				{
					f.getline(buf, BUFFER_SIZE);
					mesh->name = buf;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_PARENT")))
				{
					f.getline(buf, BUFFER_SIZE);
					mesh->parentName = buf;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_TM")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						if (buf[0] == FE_TEXT('}'))			break;
						else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
						else if (buf[0] != FE_TEXT('*'))	continue;

						/*if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
						{
							f.getline(buf, BUFFER_SIZE);
						}*/
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("INHERIT_POS")))
						{
							f >> a >> b >> c;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("INHERIT_ROT")))
						{
							f >> a >> b >> c;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("INHERIT_SCL")))
						{
							f >> a >> b >> c;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW0")))
						{
							f >> mesh->TM.m[0][0] >> mesh->TM.m[0][2] >> mesh->TM.m[0][1];
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW1")))
						{
							f >> mesh->TM.m[2][0] >> mesh->TM.m[2][2] >> mesh->TM.m[2][1];
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW2")))
						{
							f >> mesh->TM.m[1][0] >> mesh->TM.m[1][2] >> mesh->TM.m[1][1];
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW3")))
						{
							f >> mesh->TM.m[3][0] >> mesh->TM.m[3][2] >> mesh->TM.m[3][1];
							m = FEMatrix::Inverse(mesh->TM);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_POS")))
						{
							f >> mesh->vPosWorld.x >> mesh->vPosWorld.z >> mesh->vPosWorld.y;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTAXIS")))
						{
							f >> x >> z >> y;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTANGLE")))
						{
							f >> w;
							FEVector3(x, y, z) != FEVector3::Zero ? mesh->qRotWorld = FEMath::FEQuaternionRotationAxis(FEVector3(x, y, z), w) : mesh->qRotWorld = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALE")))
						{
							f >> mesh->vScaleWorld.x >> mesh->vScaleWorld.z >> mesh->vScaleWorld.y;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXIS")))
						{
							f >> x >> z >> y;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXISANG")))
						{
							f >> w;
							FEVector3(x, y, z) != FEVector3::Zero ? mesh->qScaleRotWorld = FEMath::FEQuaternionRotationAxis(FEVector3(x, y, z), w) : mesh->qScaleRotWorld = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
						}
					}
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						if (buf[0] == FE_TEXT('}'))			break;
						else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
						else if (buf[0] != FE_TEXT('*'))	continue;

						if (TCSCMP_SAME(buf + 1, FE_TEXT("TIMEVALUE")))
						{
							f >> i;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMVERTEX")))
						{
							f >> i;
							vPos.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMFACES")))
						{
							f >> i;
							iPos.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEX_LIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> i >> x >> z >> y;
								vPos[i].x = x;
								vPos[i].y = y;
								vPos[i].z = z;
								vPos[i] = vPos[i] * m;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACE_LIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> buf;

								i = 0;
								while (buf[i] != L'\0') i++;
								buf[--i] = L'\0';
								i = _wtoi(buf);

								f >> buf >> a >> buf >> b >> buf >> c;

								iPos[i].a = a;
								iPos[i].b = b;
								iPos[i].c = c;

								f.getline(buf, BUFFER_SIZE);
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVERTEX")))
						{
							f >> i;
							vTex.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_TVERTLIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> i >> x >> y >> z;
								vTex[i].x = x;
								vTex[i].y = -y;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVFACES")))
						{
							f >> i;
							iTex.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_TFACELIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> i >> a >> b >> c;
								iTex[i].a = a;
								iTex[i].b = b;
								iTex[i].c = c;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVERTEX")))
						{
							f >> i;
							vColor.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CVERTLIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> i >> x >> y >> z;
								vColor[i].x = x;
								vColor[i].y = y;
								vColor[i].z = z;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVFACES")))
						{
							f >> i;
							iColor.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CFACELIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> i >> a >> b >> c;

								iColor[i].a = a;
								iColor[i].b = b;
								iColor[i].c = c;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NORMALS")))
						{
							int normalCount = 0;

							f >> buf;

							vVertexNormal.resize(iPos.size() * 3);
							vFaceNormal.resize(iPos.size());

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
								else if (buf[0] != FE_TEXT('*'))	continue;

								if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACENORMAL")))
								{
									f >> i >> x >> z >> y;

									vFaceNormal[i].x = x;
									vFaceNormal[i].y = y;
									vFaceNormal[i].z = z;
								}
								else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEXNORMAL")))
								{
									f >> i >> x >> z >> y;

									vVertexNormal[normalCount].x = x;
									vVertexNormal[normalCount].y = y;
									vVertexNormal[normalCount].z = z;

									normalCount++;
								}
							}
						}
					}
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_MOTIONBLUR")))
				{
					f >> i;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_CASTSHADOW")))
				{
					f >> i;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_RECVSHADOW")))
				{
					f >> i;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ANIMATION")))
				{
					mesh->useAnim = true;
					
					f >> buf;

					while (true)
					{
						FEVector4 Q;
						f >> buf;

						if (buf[0] == FE_TEXT('}'))			break;
						else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);
						else if (buf[0] != FE_TEXT('*'))	continue;
						if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
						{
							f.getline(buf, BUFFER_SIZE);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("CONTROL_POS_TRACK")))
						{
							f >> buf;
							
							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								ASE_ANIM_POS animPos;
								f >> animPos.animTime >> animPos.pos.x >> animPos.pos.z >> animPos.pos.y;
								animPos.animTime = animPos.animTime / (frameSpeed * ticksPerFrame);
								mesh->animPos.push_back(animPos);
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("CONTROL_ROT_TRACK")))
						{
							f >> buf;

							ASE_ANIM_ROT animRot;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								f >> animRot.animTime >> x >> z >> y >> w;
								animRot.animTime = animRot.animTime / (frameSpeed * ticksPerFrame);
								FEVector3(x, y, z) != FEVector3::Zero ? Q = FEMath::FEQuaternionRotationAxis(FEVector3(x, y, z), w) : Q = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
								animRot.qRot = FEMath::FEQuaternionMultiply(animRot.qRot, Q);
								mesh->animRot.push_back(animRot);
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("CONTROL_SCALE_TRACK")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))			break;
								else if (buf[0] == FE_TEXT('{'))	EscapeBlock(f);

								ASE_ANIM_SCALE animScale;
								f >> animScale.animTime >> animScale.scale.x >> animScale.scale.z >> animScale.scale.y >> x >> z >> y >> w;
								animScale.animTime = animScale.animTime / (frameSpeed * ticksPerFrame);
								FEVector3(x, y, z) != FEVector3::Zero ? Q = FEMath::FEQuaternionRotationAxis(FEVector3(x, y, z), w) : Q = FEVector4(0.0f, 0.0f, 0.0f, 1.0f);
								animScale.qScaleRot = FEMath::FEQuaternionMultiply(animScale.qScaleRot, Q);
								mesh->animScale.push_back(animScale);
							}
						}
					}
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_REF")))
				{
					f >> i;
					mesh->mtrlID = vMtrl[i];
				}
			}

			if (vPos.size() != 0)			mesh->vf |= FE_VF_POSITION;
			if (vColor.size() != 0)			mesh->vf |= FE_VF_VERTEX_COLOR;
			if (vVertexNormal.size() != 0)	mesh->vf |= FE_VF_NORMAL;
			if (vTex.size() != 0)			mesh->vf |= FE_VF_TEXTURE;

			std::vector<FE_VF_PositionColorNormalTexture> verts;
			std::vector<FE_IndexFormat> indics;
			verts.resize(iPos.size() * 3);
			indics.resize(iPos.size());

			// 버텍스 정보 정리
			UINT vNum = 0;
			for (i = 0; i < iPos.size(); i++)
			{
				if ((mesh->vf & FE_VF_POSITION) == FE_VF_POSITION)
				{
					verts[vNum].position.x = vPos[iPos[i].a].x;
					verts[vNum].position.y = vPos[iPos[i].a].y;
					verts[vNum].position.z = vPos[iPos[i].a].z;
				}
				if ((mesh->vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
				{
					verts[vNum].color.x = vColor[iColor[i].a].x;
					verts[vNum].color.y = vColor[iColor[i].a].y;
					verts[vNum].color.z = vColor[iColor[i].a].z;
					verts[vNum].color.w = vColor[iColor[i].a].w;
				}
				if ((mesh->vf & FE_VF_NORMAL) == FE_VF_NORMAL)
				{
					verts[vNum].normal.x = vVertexNormal[vNum].x;
					verts[vNum].normal.y = vVertexNormal[vNum].y;
					verts[vNum].normal.z = vVertexNormal[vNum].z;
				}
				if ((mesh->vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
				{
					verts[vNum].textureCoordinate.x = vTex[iTex[i].a].x;
					verts[vNum].textureCoordinate.y = vTex[iTex[i].a].y;
				}

				indics[i].a = vNum++;

				/////////////////////////////////////////////////////////////////////////////////////

				if ((mesh->vf & FE_VF_POSITION) == FE_VF_POSITION)
				{
					verts[vNum].position.x = vPos[iPos[i].b].x;
					verts[vNum].position.y = vPos[iPos[i].b].y;
					verts[vNum].position.z = vPos[iPos[i].b].z;
				}
				if ((mesh->vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
				{
					verts[vNum].color.x = vColor[iColor[i].b].x;
					verts[vNum].color.y = vColor[iColor[i].b].y;
					verts[vNum].color.z = vColor[iColor[i].b].z;
					verts[vNum].color.w = vColor[iColor[i].b].w;
				}
				if ((mesh->vf & FE_VF_NORMAL) == FE_VF_NORMAL)
				{
					verts[vNum].normal.x = vVertexNormal[vNum].x;
					verts[vNum].normal.y = vVertexNormal[vNum].y;
					verts[vNum].normal.z = vVertexNormal[vNum].z;
				}
				if ((mesh->vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
				{
					verts[vNum].textureCoordinate.x = vTex[iTex[i].b].x;
					verts[vNum].textureCoordinate.y = vTex[iTex[i].b].y;
				}

				indics[i].b = vNum++;

				/////////////////////////////////////////////////////////////////////////////////////

				if ((mesh->vf & FE_VF_POSITION) == FE_VF_POSITION)
				{
					verts[vNum].position.x = vPos[iPos[i].c].x;
					verts[vNum].position.y = vPos[iPos[i].c].y;
					verts[vNum].position.z = vPos[iPos[i].c].z;
				}
				if ((mesh->vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
				{
					verts[vNum].color.x = vColor[iColor[i].c].x;
					verts[vNum].color.y = vColor[iColor[i].c].y;
					verts[vNum].color.z = vColor[iColor[i].c].z;
					verts[vNum].color.w = vColor[iColor[i].c].w;
				}
				if ((mesh->vf & FE_VF_NORMAL) == FE_VF_NORMAL)
				{
					verts[vNum].normal.x = vVertexNormal[vNum].x;
					verts[vNum].normal.y = vVertexNormal[vNum].y;
					verts[vNum].normal.z = vVertexNormal[vNum].z;
				}
				if ((mesh->vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
				{
					verts[vNum].textureCoordinate.x = vTex[iTex[i].c].x;
					verts[vNum].textureCoordinate.y = vTex[iTex[i].c].y;
				}

				indics[i].c = vNum++;
			}

			// 버텍스 압축
			VertexWeld(verts, indics);

			if (verts.size() == 1)
			{
				verts.clear();
				indics.clear();
			}

			mesh->index = indics;

			mesh->position.resize(verts.size());
			mesh->color.resize(verts.size());
			mesh->normal.resize(verts.size());
			mesh->texcoord.resize(verts.size());
			for (i = 0; i < verts.size(); i++)
			{
				mesh->position[i] = verts[i].position;
				mesh->color[i] = verts[i].color;
				mesh->normal[i] = Normalize(verts[i].normal);
				mesh->texcoord[i] = verts[i].textureCoordinate;
			}

			meshList.push_back(mesh);
		}
	}

	// 메쉬 계층구조의 루트
	ASE_MESH root;
	FEMatrix m;
	root.name = FE_TEXT(" \"") + GetFileName(i_fileName) + FE_TEXT('"');

	// 매쉬 계층구조 만들기
	for (auto iter = meshList.begin(); iter != meshList.end(); iter++)
	{
		if ((*iter)->parentName.empty())
		{
			root.children.push_back((*iter));
			(*iter)->vPos = (*iter)->vPosWorld;
			(*iter)->qRot = (*iter)->qRotWorld;
			(*iter)->vScale = (*iter)->vScaleWorld;
			continue;
		}

		for (auto iter2 = meshList.begin(); iter2 != meshList.end(); iter2++)
		{
			if ((*iter)->parentName == (*iter2)->name)
			{
				m = (*iter)->TM * FEMatrix::Inverse((*iter2)->TM);
				(*iter)->vPos = (*iter)->vPosWorld * FEMatrix::Inverse((*iter2)->TM) * FEMath::FEMatrixScaling((*iter2)->vScaleWorld);
				(*iter)->qRot = FEMath::FEQuaternionMultiply((*iter)->qRotWorld, FEMath::FEQuaternionInverse((*iter2)->qRotWorld));
				(*iter)->vScale.x = (*iter)->vScaleWorld.x / (*iter2)->vScaleWorld.x;
				(*iter)->vScale.y = (*iter)->vScaleWorld.y / (*iter2)->vScaleWorld.y;
				(*iter)->vScale.z = (*iter)->vScaleWorld.z / (*iter2)->vScaleWorld.z;
				(*iter2)->children.push_back((*iter));
				break;
			}
		}
	}

	// 메쉬 데이터 쓰기
	if (root.children.size() == 1)		WriteMeshData(o, *root.children.begin(), 0);
	else								WriteMeshData(o, &root, 0);

	auto iter = meshList.begin();
	while (iter != meshList.end())
		delete (*iter++);

	f.close();
	o.close();

	return true;
}
void FEFileManager::ConvertAllFileInPath(tstring i_filePath)
{
	tstring extension;

	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((_dataPath + i_filePath + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		extension = GetFileNameExtension(fd.name);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
			{
				if (_taccess((_outPath + i_filePath + fd.name + FE_TEXT("/")).c_str(), 0))
					_tmkdir((_outPath + i_filePath + fd.name + FE_TEXT("/")).c_str());

				FEFileManager::ConvertAllFileInPath(i_filePath + fd.name + FE_TEXT("/"));
			}
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("png"))
			|| TCSCMP_SAME(extension.c_str(), FE_TEXT("jpg"))
			|| TCSCMP_SAME(extension.c_str(), FE_TEXT("bmp"))
			|| TCSCMP_SAME(extension.c_str(), FE_TEXT("dds"))
			|| TCSCMP_SAME(extension.c_str(), FE_TEXT("tga")))
		{
			if (_taccess((_outPath + i_filePath + fd.name + FE_TEXT(".fet")).c_str(), 0) == -1)
				ConvertTextureFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
		}
		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fem")))
		{
			if (_taccess((_outPath + i_filePath + fd.name).c_str(), 0) == -1)
				break;
		}
		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fx")))
		{
			//if (_taccess((_outPath + i_filePath + GetFileName(fd.name) + FE_TEXT(".vso")).c_str(), 0) == -1)
			{
				ConvertShaderFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
				CompileVertexShader(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
				CompilePixelShader(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
			}
		}
		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("vsh")))
		{
			if (_taccess((_outPath + i_filePath + GetFileName(fd.name) + FE_TEXT(".vso")).c_str(), 0) == -1)
			{
				ConvertShaderFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
				CompileVertexShader(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
			}
		}
		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("psh")))
		{
			if (_taccess((_outPath + i_filePath + GetFileName(fd.name) + FE_TEXT(".pso")).c_str(), 0) == -1)
				CompilePixelShader(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
		}
		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("ase")))
		{
			if (_taccess((_outPath + i_filePath + GetFileName(fd.name) + FE_TEXT(".msh")).c_str(), 0) == -1)
				ConvertASEMeshFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEMesh* i_pMtrl)
{
	
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEShader* i_pShader)
{
	
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEMaterial* i_pMtrl)
{

}



FEFileManager* FEFileManager::GetInstance()
{
	if (_pInstance == nullptr)
		_pInstance = new FEFileManager;

	return _pInstance;
}
void FEFileManager::Update()
{
	ConvertAllFileInPath(FE_TEXT(""));
}