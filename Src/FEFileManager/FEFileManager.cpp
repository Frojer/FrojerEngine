#include "FEFileManager.h"

#include <map>
#include <Windows.h>
#include <rpcdce.h>
#pragma comment(lib, "Rpcrt4.lib")

#include <FEUtility.h>

#ifdef _WIN32
//-------------------------------
// 윈도우 폴더 내 파일 찾기
//-------------------------------
#include <io.h>
#include <conio.h>
//-------------------------------
#else
#endif

#define BUFFER_SIZE 1024

FEFileManager* FEFileManager::_pInstance = nullptr;

FEFileManager::FEFileManager()
{

}
FEFileManager::~FEFileManager()
{

}

struct ASE_MESH
{
	tstring name;
	tstring parentName;

	UUID mtrlID;

	std::list<ASE_MESH*> children;

	unsigned char vf;

	std::vector<FEVector3>		position;
	std::vector<FE_IndexFormat>	index;
	std::vector<FEVector4>		color;
	std::vector<FEVector3>		normal;
	std::vector<FEVector2>		texcoord;

	ASE_MESH()
		: vf(0)
	{
		ZeroMemory(&mtrlID, sizeof(mtrlID));
	}
};

void WriteMeshData(tofstream& o, ASE_MESH* mesh, UINT depth)
{
#define TAP for (UINT di = 0; di < depth; di++) o << FE_TEXT('\t');
	UINT i = 0;

	TAP	o << FE_TEXT("MeshObject") << std::endl;
	TAP	o << FE_TEXT("{") << std::endl;
	depth++;

	// 메쉬 이름 쓰기
	TAP	o << FE_TEXT("Name = ") << mesh->name << std::endl;

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

	for (auto iter = mesh->children.begin(); iter != mesh->children.end(); iter++)
	{
		WriteMeshData(o, (*iter), depth);
	}

	// 머테리얼 ID 쓰기
	UUID uuid;
	ZeroMemory(&uuid, sizeof(uuid));
	if (mesh->mtrlID != uuid)
	{
		TAP	o << FE_TEXT("MaterialID = ") << std::hex << mesh->mtrlID.Data1 << mesh->mtrlID.Data2 << mesh->mtrlID.Data3;
		for (int ui = 0; ui < 8; ui++) o << (short)mesh->mtrlID.Data4[ui];
		o << std::dec << std::endl;
	}
	else	TAP	o << FE_TEXT("MaterialID = ") << 0;

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
void ConvertShaderFile(tstring i_fileName, tstring i_outPath)
{
	TCHAR str[BUFFER_SIZE];
	UINT i = 0;

	tifstream f(i_fileName.c_str());

	if (f.fail())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to open shader file."));
		return;
	}

	while (f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("VS_Main")))
		{
			if (str[7] != FE_TEXT('('))
			{
				f.getline(str, BUFFER_SIZE);
				while (true)
				{
					switch (str[i])
					{
					case FE_TEXT(' '):
						break;
					case FE_TEXT(':'):
						break;
					case FE_TEXT('('):
						break;
					case FE_TEXT(')'):
						break;
					}
				}
			}
		}
	}
}
bool ConvertASEMapFile(tifstream& f, tstring i_filePath, TCHAR buf[])
{
	int a, b, c, i = 0;
	float x, y, z, w;

	TCHAR mapClass[BUFFER_SIZE];

	f >> buf;
	// Map Name
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

	// 머테리얼 클래스에 따른 처리
	if (TCSCMP_SAME(mapClass, FE_TEXT("Bitmap")))
	{
		// BITMAP "D:\Works\Lecture01\02.ASE_Engine (ver.2.1)\Data\Box\woodbox.jpg";
		f >> buf;
		i = 0;

		f.getline(buf, BUFFER_SIZE);

		while (buf[i] != FE_TEXT('"'))	i++;
		int s = ++i;
		while (buf[i] != FE_TEXT('"'))	i++;
		int e = i;

		_tcsncpy_s(mapClass, buf + s, e - s);
		GetFilePath(i_filePath) + GetFileNameWithExtension(mapClass);
		// MAP_TYPE Screen;
		f >> buf >> buf;
		// UVW_U_OFFSET 0.0000;
		f >> buf >> x;
		// UVW_V_OFFSET 0.0000;
		f >> buf >> x;
		// UVW_U_TILING 1.0000;
		f >> buf >> x;
		// UVW_V_TILING 1.0000;
		f >> buf >> x;
		// UVW_ANGLE 0.0000;
		f >> buf >> x;
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
	}
	else	while (buf[0] != FE_TEXT('}')) f >> buf;

	return true;
}
bool ConvertASEMaterialFile(tifstream& f, tstring i_filePath, tstring i_outPath, TCHAR buf[], std::vector<UUID>& vMtrl)
{
	tofstream om;
	tstringstream ss;

	TCHAR mtrlClass[BUFFER_SIZE];

	int a, b, c, i = 0;
	float x, y, z, w;
	UUID uuid;

	FEVector4 ambient;
	FEVector4 diffuse;
	FEVector4 specular;
	float shine, shineStrength, transparency;

	f >> i >> buf;
	UuidCreate(&uuid);
	vMtrl[i] = uuid;

#pragma region MATERIAL_NAME
	{
		f >> buf;
		i = 0;

		f.getline(buf, BUFFER_SIZE);

		while (buf[i] != FE_TEXT('"'))	i++;
		int s = ++i;
		while (buf[i] != FE_TEXT('"'))	i++;
		int e = i;

		_tcsncpy_s(mtrlClass, buf + s, e - s);

		// 파일 생성
		ss << i_outPath << mtrlClass << FE_TEXT(".fem");
		om.open(ss.str());
		if (!om.is_open()) return false;
	}
#pragma endregion

	// UUID 쓰기
	om << FE_TEXT("ID = ") << std::hex << uuid.Data1 << uuid.Data2 << uuid.Data3;
	for (int j = 0; j < 8; j++) om << (short)uuid.Data4[j];
	om << std::dec << std::endl;

#pragma region MATERIAL_CLASS
	{
		f >> buf;
		i = 0;

		f.getline(buf, BUFFER_SIZE);

		while (buf[i] != FE_TEXT('"'))	i++;
		int s = ++i;
		while (buf[i] != FE_TEXT('"'))	i++;
		int e = i;

		_tcsncpy_s(mtrlClass, buf + s, e - s);
	}
#pragma endregion

	f >> buf >> ambient.x >> ambient.y >> ambient.z;
	f >> buf >> diffuse.x >> diffuse.y >> diffuse.z;
	f >> buf >> specular.x >> specular.y >> specular.z;
	f >> buf >> shine;
	f >> buf >> shineStrength;
	f >> buf >> transparency;
	// MATERIAL_WIRESIZE
	f >> buf >> x;

	// 머테리얼 클래스에 따른 처리
	if (TCSCMP_SAME(mtrlClass, FE_TEXT("Standard")))
	{
		while (true)
		{
			f >> buf;

			if (buf[0] == FE_TEXT('}'))
				break;

			else if (buf[0] != FE_TEXT('*'))
			{
				f.getline(buf, BUFFER_SIZE);
				continue;
			}

			if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_SHADING")))
			{
				f >> buf;
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_XP_FALLOFF")))
			{
				f >> x;
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_SELFILLUM")))
			{
				f >> x;
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_FALLOFF")))
			{
				f >> buf;
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_XP_TYPE")))
			{
				f >> buf;
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_DIFFUSE")))
			{
				ConvertASEMapFile(f, i_filePath, buf);
			}
		}
	}
	else	while (buf[0] != FE_TEXT('}')) f >> buf;

	// 머테리얼 정보 출력
	om << FE_TEXT("Diffuse = ") << diffuse.x << FE_TEXT('\t') << diffuse.y << FE_TEXT('\t') << diffuse.z << FE_TEXT('\t') << 1.0f - transparency << std::endl;
	om << FE_TEXT("Ambient = ") << ambient.x << FE_TEXT('\t') << ambient.y << FE_TEXT('\t') << ambient.z << FE_TEXT('\t') << 1 << std::endl;
	om << FE_TEXT("Specular = ") << specular.x << FE_TEXT('\t') << specular.y << FE_TEXT('\t') << specular.z << FE_TEXT('\t') << 1 << std::endl;
	om << FE_TEXT("Power = ") << shine * 100 << std::endl;

	om.close();
}
bool ConvertASEMeshFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tostringstream ss;
	tofstream o;
	tifstream f;

	TCHAR buf[BUFFER_SIZE];

	std::vector<UUID> vMtrl;
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

	UUID uuid;
	UuidCreate(&uuid);

	// 원본 파일과 저장할 파일에 대한 객체 생성
	o.open(i_outPath + GetFileName(i_fileName) + FE_TEXT(".msh"));
	if (!o.is_open())	return false;
	f.open(i_filePath + i_fileName);
	if (!f.is_open())	return false;

	// UUID 쓰기
	o << FE_TEXT("ID = ") << std::hex << uuid.Data1 << uuid.Data2 << uuid.Data3;
	for (int j = 0; j < 8; j++) o << (short)uuid.Data4[j];
	o << std::dec << std::endl;

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

				if (buf[0] == FE_TEXT('}'))
					break;

				else if (buf[0] != FE_TEXT('*'))
				{
					f.getline(buf, BUFFER_SIZE);
					continue;
				}

				if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FILENAME")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FIRSTFRAME")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_LASTFRAME")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_FRAMESPEED")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_TICKSPERFRAME")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_BACKGROUND_STATIC")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("SCENE_AMBIENT_STATIC")))
				{

				}
			}
		}
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_LIST")))
		{
			f >> buf;

			while (true)
			{
				f >> buf;

				if (buf[0] == FE_TEXT('}'))
					break;

				else if (buf[0] != FE_TEXT('*'))
				{
					f.getline(buf, BUFFER_SIZE);
					continue;
				}

				if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_COUNT")))
				{
					f >> i;

					vMtrl.resize(i);
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL")))
				{
					ConvertASEMaterialFile(f, i_filePath, i_outPath, buf, vMtrl);
				}
			}
		}
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("GEOMOBJECT")))
		{
			ASE_MESH* mesh = new ASE_MESH;

			f >> buf;

			while (true)
			{
				f >> buf;

				if (buf[0] == FE_TEXT('}'))
					break;

				else if (buf[0] != FE_TEXT('*'))
				{
					f.getline(buf, BUFFER_SIZE);
					continue;
				}

				if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
				{
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != FE_TEXT('"'))	i++;
					int s = ++i;
					while (buf[i] != FE_TEXT('"'))	i++;
					int e = i;

					_tcsncpy_s(buf, buf + s, e - s);
					mesh->name = buf;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_PARENT")))
				{
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != FE_TEXT('"'))	i++;
					int s = ++i;
					while (buf[i] != FE_TEXT('"'))	i++;
					int e = i;

					_tcsncpy_s(buf, buf + s, e - s);
					mesh->parentName = buf;
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_TM")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						if (buf[0] == FE_TEXT('}'))
							break;

						else if (buf[0] != FE_TEXT('*'))
						{
							f.getline(buf, BUFFER_SIZE);
							continue;
						}

						/*if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
						{
							TCHAR name[BUFFER_SIZE];
							i = 0;

							f.getline(buf, BUFFER_SIZE);

							while (buf[i] != FE_TEXT('"'))	i++;
							int s = ++i;
							while (buf[i] != FE_TEXT('"'))	i++;
							int e = i;

							_tcsncpy_s(name, buf + s, e - s);
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
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW1")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW2")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROW3")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_POS")))
						{
							f >> pos.x >> pos.y >> pos.z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTAXIS")))
						{
							f >> rot.x >> rot.y >> rot.z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTANGLE")))
						{
							f >> rotAngle;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALE")))
						{
							f >> scale.x >> scale.y >> scale.z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXIS")))
						{
							f >> scaleAxis.x >> scaleAxis.y >> scaleAxis.z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXISANG")))
						{
							f >> scaleAxisAngle;
						}
					}
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH")))
				{
					f >> buf;

					while (true)
					{
						f >> buf;

						if (buf[0] == FE_TEXT('}'))
							break;

						else if (buf[0] != FE_TEXT('*'))
						{
							f.getline(buf, BUFFER_SIZE);
							continue;
						}

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

								if (buf[0] == FE_TEXT('}'))
									break;

								f >> i >> x >> y >> z;
								vPos[i].x = x;
								vPos[i].y = y;
								vPos[i].z = z;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACE_LIST")))
						{
							f >> buf;

							while (true)
							{
								f >> buf;

								if (buf[0] == FE_TEXT('}'))
									break;

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

								if (buf[0] == FE_TEXT('}'))
									break;

								f >> i >> x >> y >> z;
								vTex[i].x = x;
								vTex[i].y = y;
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

								if (buf[0] == FE_TEXT('}'))
									break;

								f >> i >> a >> b >> c;
								iTex[i].a = a;
								iTex[i].b = b;
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

								if (buf[0] == FE_TEXT('}'))
									break;

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

								if (buf[0] == FE_TEXT('}'))
									break;

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

								if (buf[0] == FE_TEXT('}'))
									break;

								else if (buf[0] != FE_TEXT('*'))
								{
									f.getline(buf, BUFFER_SIZE);
									continue;
								}


								if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACENORMAL")))
								{
									f >> i >> x >> y >> z;

									vFaceNormal[i].x = x;
									vFaceNormal[i].y = y;
									vFaceNormal[i].z = z;
								}
								else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEXNORMAL")))
								{
									f >> i >> x >> y >> z;

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
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("MATERIAL_REF")))
				{
					f >> i;
					mesh->mtrlID = vMtrl[i];
				}
			}

			if (iPos.size() != 0)			mesh->vf |= FE_VF_POSITION;
			if (iColor.size() != 0)			mesh->vf |= FE_VF_VERTEX_COLOR;
			if (vVertexNormal.size() != 0)	mesh->vf |= FE_VF_NORMAL;
			if (iTex.size() != 0)			mesh->vf |= FE_VF_TEXTURE;

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
					verts[vNum].textureCoordinate.y = -vTex[iTex[i].a].y;
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
					verts[vNum].textureCoordinate.y = -vTex[iTex[i].b].y;
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
					verts[vNum].textureCoordinate.y = -vTex[iTex[i].c].y;
				}

				indics[i].c = vNum++;
			}

			// 버텍스 압축
			VertexWeld(verts, indics);

			mesh->index = indics;

			mesh->position.resize(verts.size());
			mesh->color.resize(verts.size());
			mesh->normal.resize(verts.size());
			mesh->texcoord.resize(verts.size());
			for (i = 0; i < verts.size(); i++)
			{
				mesh->position[i] = verts[i].position;
				mesh->color[i] = verts[i].color;
				mesh->normal[i] = verts[i].normal;
				mesh->texcoord[i] = verts[i].textureCoordinate;
			}

			meshList.push_back(mesh);
		}
	}

	// 메쉬 계층구조의 루트
	ASE_MESH root;
	root.name = GetFileName(i_fileName);

	// 매쉬 계층구조 만들기
	for (auto iter = meshList.begin(); iter != meshList.end(); iter++)
	{
		if ((*iter)->parentName.empty())
		{
			root.children.push_back((*iter));
			continue;
		}

		for (auto iter2 = meshList.begin(); iter2 != meshList.end(); iter2++)
		{
			if ((*iter)->parentName == (*iter2)->name)
			{
				(*iter2)->children.push_back((*iter));
				break;
			}
		}
	}

	WriteMeshData(o, &root, 0);

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

#ifdef _WIN32
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
				FEFileManager::ConvertAllFileInPath(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fx")))
		{
			if (_taccess((_outPath + i_filePath + fd.name + FE_TEXT(".fes")).c_str(), 0) == -1)
				break;

			ConvertShaderFile(_dataPath + i_filePath + fd.name, _outPath + i_filePath);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("ase")))
		{
			if (_taccess((_outPath + i_filePath + GetFileName(fd.name) + FE_TEXT(".msh")).c_str(), 0) == -1)
				ConvertASEMeshFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEMesh* i_pMtrl)
{
	
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEShader* i_pShader)
{
	/*tofstream f(i_filePath + i_pShader->m_Name + FE_TEXT(".fem"));

	f << FE_TEXT("Semantics = ") << i_pShader->GetSemantics() << std::endl;
	f << FE_TEXT("CountTexture = ") << i_pShader->GetCountTexture() << std::endl;
	f << FE_TEXT("CountMatrix = ") << i_pShader->GetCountMatrix() << std::endl;
	f << FE_TEXT("CountVector = ") << i_pShader->GetCountVector() << std::endl;
	f << FE_TEXT("CountScalar = ") << i_pShader->GetCountScalar() << std::endl;
	f << FE_TEXT("UseLight = ") << i_pShader->IsUseLight();

	f.close();*/
}
void FEFileManager::ExportFile(tstring i_filePath, tstring i_outPath, const FEMaterial* i_pMtrl)
{
	tofstream f(i_filePath + i_pMtrl->m_Name + FE_TEXT(".fem"));

	f << FE_TEXT("Shader = ") << i_pMtrl->GetShader()->m_Name << std::endl;

	f.close();
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