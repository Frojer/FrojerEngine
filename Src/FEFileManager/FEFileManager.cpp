#include "FEFileManager.h"

#include <map>

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
	INT64 ID;
	tstring name;
	tstring parentName;

	INT64 mtrlID;

	std::list<ASE_MESH*> children;

	unsigned char vf;

	std::vector<FEVector3>		position;
	std::vector<FE_IndexFormat>	index;
	std::vector<FEVector4>		color;
	std::vector<FEVector3>		normal;
	std::vector<FEVector2>		texcoord;

	ASE_MESH()
		: ID(0), mtrlID(0), vf(0)
	{
		
	}
};

void ConvertShaderFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{

}
void CompileVertexShader(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tstring command = FE_TEXT("../../SDK/FileConverter/fxc.exe");

	command.append(FE_TEXT(" /E VS_Main"));
	command.append(FE_TEXT(" /T vs_5_0"));
	command.append(FE_TEXT(" /Fo ") + i_outPath + GetFileName(i_fileName) + FE_TEXT(".vso"));
	command.append(i_outPath + i_fileName);
	// 열기준 연산 행기준은 /Zpc
	command.append(FE_TEXT(" /Zpr"));

	_tsystem(command.c_str());
}
void CompilePixelShader(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tstring command = FE_TEXT("../../SDK/FileConverter/fxc.exe");

	command.append(FE_TEXT(" /E PS_Main"));
	command.append(FE_TEXT(" /T ps_5_0"));
	command.append(FE_TEXT(" /Fo ") + i_outPath + GetFileName(i_fileName) + FE_TEXT(".pso"));
	command.append(i_outPath + i_fileName);
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
void ConvertASEMapFile(tifstream& f, tofstream& o, tstring i_filePath, tstring i_outPath, TCHAR buf[])
{
	int a, b, c, i = 0;
	float x, y, z, w;

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

		tifstream mif(i_filePath + GetFileNameWithExtension(mapClass), std::ios::binary);

		if (mif.is_open())
		{
			tofstream mof(i_outPath + GetFileNameWithExtension(mapClass), std::ios::binary);

			if (mof.is_open())
			{
				FECopyFile(mif, mof);
			}
		}

		o << FE_TEXT('\t') << FE_TEXT('\t') << FE_TEXT("Map = ") << FE_TEXT('"') + i_outPath + GetFileNameWithExtension(mapClass) + FE_TEXT('"') << std::endl;
		
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
		f >> buf >> x;
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
	else	while (buf[0] != FE_TEXT('}')) f >> buf;

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
	o << FE_TEXT('\t') << FE_TEXT("Ambient") << FE_TEXT('\t') << ambient.x << FE_TEXT('\t') << ambient.y << FE_TEXT('\t') << ambient.z << FE_TEXT('\t') << std::endl;
	o << FE_TEXT('\t') << FE_TEXT("Specular") << FE_TEXT('\t') << specular.x << FE_TEXT('\t') << specular.y << FE_TEXT('\t') << specular.z << FE_TEXT('\t') << std::endl;
	o << FE_TEXT('\t') << FE_TEXT("Power = ") << shine * 100 << std::endl;

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

			else if (buf[0] != FE_TEXT('*'))
			{
				f.getline(buf, BUFFER_SIZE);
				continue;
			}
			
			if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_DIFFUSE")))
			{
				o << FE_TEXT('\t') << FE_TEXT("DiffuseMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SPECULAR")))
			{
				o << FE_TEXT('\t') << FE_TEXT("SpecularMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SHINE")))
			{
				o << FE_TEXT('\t') << FE_TEXT("ShineMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SHINESTRENGTH")))
			{
				o << FE_TEXT('\t') << FE_TEXT("ShineStrengthMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_SELFILLUM")))
			{
				o << FE_TEXT('\t') << FE_TEXT("SelfIllumMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_OPACITY")))
			{
				o << FE_TEXT('\t') << FE_TEXT("OpacityMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_FILTERCOLOR")))
			{
				o << FE_TEXT('\t') << FE_TEXT("FilterColorMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_BUMP")))
			{
				o << FE_TEXT('\t') << FE_TEXT("BumpMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_REFLECT")))
			{
				o << FE_TEXT('\t') << FE_TEXT("ReflectMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
			else if (TCSCMP_SAME(buf + 1, FE_TEXT("MAP_REFRACT")))
			{
				o << FE_TEXT('\t') << FE_TEXT("RefractMap") << std::endl;
				ConvertASEMapFile(f, o, i_filePath, i_outPath, buf);
			}
		}
	}
	else	while (buf[0] != FE_TEXT('}')) f >> buf;
	o << FE_TEXT("}") << std::endl;
}
bool ConvertASEMeshFile(tstring i_filePath, tstring i_outPath, tstring i_fileName)
{
	tostringstream ss;
	tofstream o;
	tifstream f;

	TCHAR buf[BUFFER_SIZE];

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
					ConvertASEMaterialFile(f, o, i_filePath, i_outPath, buf, vMtrl);
				}
			}
		}
		else if (TCSCMP_SAME(buf + 1, FE_TEXT("GEOMOBJECT")))
		{
			ASE_MESH* mesh = new ASE_MESH;

			mesh->ID = CreateUUIDHashCode64();

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

						if (buf[0] == FE_TEXT('}'))
							break;

						else if (buf[0] != FE_TEXT('*'))
						{
							f.getline(buf, BUFFER_SIZE);
							continue;
						}

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
	root.name = FE_TEXT(" \"") + GetFileName(i_fileName) + FE_TEXT('"');

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

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fx")) || TCSCMP_SAME(extension.c_str(), FE_TEXT("vsh")))
		{
			if (_taccess((_outPath + i_filePath + fd.name + FE_TEXT(".fes")).c_str(), 0) == -1)
				break;

			ConvertShaderFile(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
			CompileVertexShader(_dataPath + i_filePath, _outPath + i_filePath, fd.name);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("psh")))
		{
			if (_taccess((_outPath + i_filePath + fd.name + FE_TEXT(".fes")).c_str(), 0) == -1)
				break;

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