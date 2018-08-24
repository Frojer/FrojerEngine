#include "FEFileManager.h"

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

void ConvertShaderFile(tstring i_fileName)
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
void ConvertASEMeshFile(tstring i_fileName)
{
	TCHAR buf[BUFFER_SIZE];
	TCHAR name[BUFFER_SIZE];
	TCHAR parentName[BUFFER_SIZE];

	FEVector3 pos, rot, scale;
	FEVector3 scaleAxis;
	float rotAngle, scaleAxisAngle;

	tifstream f(i_fileName);

	std::vector<FEVector4> vecColor;
	std::vector<FEVector3> vecPos;
	std::vector<FEVector3> vecNor;
	std::vector<FEVector2> vecTex;
	std::vector<IndexFormat> vecIndex;

	int a, b, c, i;
	float x, y, z, w;

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

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

						if (TCSCMP_SAME(buf + 1, FE_TEXT("NODE_NAME")))
						{
							TCHAR name[BUFFER_SIZE];
							i = 0;

							f.getline(buf, BUFFER_SIZE);

							while (buf[i] != FE_TEXT('"'))	i++;
							int s = ++i;
							while (buf[i] != FE_TEXT('"'))	i++;
							int e = i;

							_tcsncpy_s(name, buf + s, e - s);
						}
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
							vecPos.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMFACES")))
						{
							f >> i;
							vecIndex.resize(i);
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
								vecPos[i].x = x;
								vecPos[i].y = y;
								vecPos[i].z = z;
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

								vecIndex[i].x = a;
								vecIndex[i].y = b;
								vecIndex[i].z = c;

								f.getline(buf, BUFFER_SIZE);
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVERTEX")))
						{
							f >> i;
							vecTex.resize(i);
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVERTEX")))
						{
							f >> i;
							vecColor.resize(i);
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
								vecColor[i].x = x;
								vecColor[i].y = y;
								vecColor[i].z = z;
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVFACES")))
						{
							f >> i;
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
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NORMALS")))
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


								if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_FACENORMAL")))
								{
									f >> i >> x >> y >> z;
								}
								else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEXNORMAL")))
								{
									f >> i >> x >> y >> z;
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
}
void FEFileManager::ConvertAllFileInPath(tstring i_filePath)
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
		FileNameExtension(fd.name, extension);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
				FEFileManager::ConvertAllFileInPath(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fx")))
		{
			ConvertShaderFile(i_filePath + fd.name);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("ase")))
		{
			ConvertASEMeshFile(i_filePath + fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FEFileManager::ExportFile(tstring i_filePath, const FEMesh* i_pMtrl)
{
	
}
void FEFileManager::ExportFile(tstring i_filePath, const FEShader* i_pShader)
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
void FEFileManager::ExportFile(tstring i_filePath, const FEMaterial* i_pMtrl)
{
	tofstream f(i_filePath + i_pMtrl->m_Name + FE_TEXT(".fem"));

	f << FE_TEXT("Shader = ") << i_pMtrl->GetShader()->m_Name << std::endl;

	f.close();
}

void ImportShader(tstring i_path)
{
	tstring extension;

#ifdef _WIN32
	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((i_path + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		FileNameExtension(fd.name, extension);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
				ImportShader(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fes")))
		{
			
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
}