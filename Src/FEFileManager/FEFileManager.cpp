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
bool ConvertASEMeshFile(tstring i_fileName, tstring i_outPath)
{
	tostringstream ss;
	tofstream o;
	tifstream f;

	UUID uuid;
	UuidCreate(&uuid);

	TCHAR buf[BUFFER_SIZE];
	TCHAR name[BUFFER_SIZE];
	TCHAR parentName[BUFFER_SIZE];

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

	// 임시 파일 생성
	while (true)
	{
		ss.clear();
		ss.str(FE_TEXT(""));
		ss << i_outPath << FE_TEXT("temp") << i++ << FE_TEXT(".msh");
		if (_taccess(ss.str().c_str(), 0) == -1)		break;
	}

	// 원본 파일과 저장할 파일에 대한 객체 생성
	o.open(ss.str());
	if (!o.is_open())	return false;
	f.open(i_fileName);
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

		else if (TCSCMP_SAME(buf + 1, FE_TEXT("GEOMOBJECT")))
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

				if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
				{
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != FE_TEXT('"'))	i++;
					int s = ++i;
					while (buf[i] != FE_TEXT('"'))	i++;
					int e = i;

					_tcsncpy_s(name, buf + s, e - s);
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_PARENT")))
				{
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != FE_TEXT('"'))	i++;
					int s = ++i;
					while (buf[i] != FE_TEXT('"'))	i++;
					int e = i;

					_tcsncpy_s(parentName, buf + s, e - s);
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
									vFaceNormal[i].y = z;
									vFaceNormal[i].z = y;
								}
								else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEXNORMAL")))
								{
									f >> i >> x >> y >> z;

									vVertexNormal[normalCount].x = x;
									vVertexNormal[normalCount].y = z;
									vVertexNormal[normalCount].z = y;

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
			}
		}
	}

	unsigned char vf = 0;

	if (iPos.size() != 0)			vf |= FE_VF_POSITION;
	if (iColor.size() != 0)			vf |= FE_VF_VERTEX_COLOR;
	if (vVertexNormal.size() != 0)	vf |= FE_VF_NORMAL;
	if (iTex.size() != 0)			vf |= FE_VF_TEXTURE;

	o << FE_TEXT("VertexFormat = ") << vf << std::endl;

	///
	std::vector<FE_VF_PositionColorNormalTexture> verts;
	std::vector<FE_IndexFormat> indics;
	verts.resize(iPos.size() * 3);
	indics.resize(iPos.size());
	
	// 버텍스 정보 정리
	UINT vNum = 0;
	for (i = 0; i < iPos.size(); i++)
	{
		if ((vf & FE_VF_POSITION) == FE_VF_POSITION)
		{
			verts[vNum].position.x = vPos[iPos[i].a].x;
			verts[vNum].position.y = vPos[iPos[i].a].y;
			verts[vNum].position.z = vPos[iPos[i].a].z;
		}
		if ((vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
		{
			verts[vNum].color.x = vColor[iColor[i].a].x;
			verts[vNum].color.y = vColor[iColor[i].a].y;
			verts[vNum].color.z = vColor[iColor[i].a].z;
			verts[vNum].color.w = vColor[iColor[i].a].w;
		}
		if ((vf & FE_VF_NORMAL) == FE_VF_NORMAL)
		{
			verts[vNum].normal.x = vVertexNormal[vNum].x;
			verts[vNum].normal.y = vVertexNormal[vNum].y;
			verts[vNum].normal.z = vVertexNormal[vNum].z;
		}
		if ((vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
		{
			verts[vNum].textureCoordinate.x = vTex[iTex[i].a].x;
			verts[vNum].textureCoordinate.y = -vTex[iTex[i].a].y;
		}

		indics[i].a = vNum++;

		/////////////////////////////////////////////////////////////////////////////////////

		if ((vf & FE_VF_POSITION) == FE_VF_POSITION)
		{
			verts[vNum].position.x = vPos[iPos[i].b].x;
			verts[vNum].position.y = vPos[iPos[i].b].y;
			verts[vNum].position.z = vPos[iPos[i].b].z;
		}
		if ((vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
		{
			verts[vNum].color.x = vColor[iColor[i].b].x;
			verts[vNum].color.y = vColor[iColor[i].b].y;
			verts[vNum].color.z = vColor[iColor[i].b].z;
			verts[vNum].color.w = vColor[iColor[i].b].w;
		}
		if ((vf & FE_VF_NORMAL) == FE_VF_NORMAL)
		{
			verts[vNum].normal.x = vVertexNormal[vNum].x;
			verts[vNum].normal.y = vVertexNormal[vNum].y;
			verts[vNum].normal.z = vVertexNormal[vNum].z;
		}
		if ((vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
		{
			verts[vNum].textureCoordinate.x = vTex[iTex[i].b].x;
			verts[vNum].textureCoordinate.y = -vTex[iTex[i].b].y;
		}

		indics[i].b = vNum++;

		/////////////////////////////////////////////////////////////////////////////////////

		if ((vf & FE_VF_POSITION) == FE_VF_POSITION)
		{
			verts[vNum].position.x = vPos[iPos[i].c].x;
			verts[vNum].position.y = vPos[iPos[i].c].y;
			verts[vNum].position.z = vPos[iPos[i].c].z;
		}
		if ((vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)
		{
			verts[vNum].color.x = vColor[iColor[i].c].x;
			verts[vNum].color.y = vColor[iColor[i].c].y;
			verts[vNum].color.z = vColor[iColor[i].c].z;
			verts[vNum].color.w = vColor[iColor[i].c].w;
		}
		if ((vf & FE_VF_NORMAL) == FE_VF_NORMAL)
		{
			verts[vNum].normal.x = vVertexNormal[vNum].x;
			verts[vNum].normal.y = vVertexNormal[vNum].y;
			verts[vNum].normal.z = vVertexNormal[vNum].z;
		}
		if ((vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)
		{
			verts[vNum].textureCoordinate.x = vTex[iTex[i].c].x;
			verts[vNum].textureCoordinate.y = -vTex[iTex[i].c].y;
		}

		indics[i].c = vNum++;
	}

	// 버텍스 압축
	VertexWeld(verts, indics);

	// 버텍스 정보들 쓰기
	o << FE_TEXT("VertexCount = ") << verts.size() << std::endl;

	for (i = 0; i < verts.size(); i++)
	{
		if ((vf & FE_VF_POSITION) == FE_VF_POSITION)	o << verts[i].position.x << FE_TEXT("\t") << verts[i].position.y << FE_TEXT("\t") << verts[i].position.z << std::endl;
		if ((vf & FE_VF_VERTEX_COLOR) == FE_VF_VERTEX_COLOR)	o << verts[i].color.x << FE_TEXT("\t") << verts[i].color.y << FE_TEXT("\t") << verts[i].color.z << FE_TEXT("\t") << verts[i].color.w << std::endl;
		if ((vf & FE_VF_NORMAL) == FE_VF_NORMAL)	o << verts[i].normal.x << FE_TEXT("\t") << verts[i].normal.y << FE_TEXT("\t") << verts[i].normal.z << std::endl;
		if ((vf & FE_VF_TEXTURE) == FE_VF_TEXTURE)	o << verts[i].textureCoordinate.x << FE_TEXT("\t") << verts[i].textureCoordinate.y << std::endl;
	}

	// 인덱스 정보들 쓰기
	o << FE_TEXT("IndexCount = ") << indics.size() << std::endl;

	for (i = 0; i < indics.size(); i++)
	{
		o << indics[i].a << FE_TEXT("\t") << indics[i].b << FE_TEXT("\t") << indics[i].c << std::endl;
	}

	f.close();
	o.close();

	_trename(ss.str().c_str(), (i_outPath + name + FE_TEXT(".msh")).c_str());

	return true;
}
void FEFileManager::ConvertAllFileInPath(tstring i_filePath, tstring i_outPath)
{
	tstring extension;

#ifdef _WIN32
	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((i_filePath + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		extension = GetFileNameExtension(fd.name);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
				FEFileManager::ConvertAllFileInPath(i_filePath + fd.name + FE_TEXT("/"), i_outPath);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fx")))
		{
			ConvertShaderFile(i_filePath + fd.name, i_outPath);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("ase")))
		{
			ConvertASEMeshFile(i_filePath + fd.name, i_outPath);
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