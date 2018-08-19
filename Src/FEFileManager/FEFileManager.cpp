#include "FEFileManager.h"
#include <FEShader.h>
#include <FEMaterial.h>
#include <FEMesh.h>

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

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fes")))
		{
			FEFileManager::ConvertShaderFile(i_filePath + fd.name);
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("ase")))
		{
			FEFileManager::ConvertASEMeshFile(i_filePath + fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FEFileManager::ConvertShaderFile(tstring i_fileName)
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
void FEFileManager::ConvertASEMeshFile(tstring i_fileName)
{
	TCHAR buf[BUFFER_SIZE];
	tifstream f(i_fileName);

	std::vector<FEVector3> vecPos;
	std::vector<FEVector3> vecNor;
	std::vector<FEVector2> vecTex;

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
					TCHAR name[BUFFER_SIZE];
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
					TCHAR name[BUFFER_SIZE];
					i = 0;

					f.getline(buf, BUFFER_SIZE);

					while (buf[i] != FE_TEXT('"'))	i++;
					int s = ++i;
					while (buf[i] != FE_TEXT('"'))	i++;
					int e = i;

					_tcsncpy_s(name, buf + s, e - s);
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
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTAXIS")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_ROTANGLE")))
						{
							f >> x;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALE")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXIS")))
						{
							f >> x >> y >> z;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("TM_SCALEAXISANG")))
						{
							f >> x;
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
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMFACES")))
						{
							f >> i;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEX_LIST")))
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

								if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_VERTEX")))
								{
									f >> i >> x >> y >> z;
								}
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

								else if (buf[0] != FE_TEXT('*'))
								{
									f.getline(buf, BUFFER_SIZE);
									continue;
								}
							}
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMTVERTEX")))
						{
							f >> i;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVERTEX")))
						{
							f >> i;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CVERTLIST")))
						{

						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NUMCVFACES")))
						{
							f >> i;
						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_CFACELIST")))
						{

						}
						else if (TCSCMP_SAME(buf + 1, FE_TEXT("MESH_NORMALS")))
						{

						}
					}
				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_MOTIONBLUR")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_CASTSHADOW")))
				{

				}
				else if (TCSCMP_SAME(buf + 1, FE_TEXT("PROP_RECVSHADOW")))
				{

				}
			}
		}
	}
}

void LoadShader(tstring i_shaderPath)
{
	FEShader* pShader;
	TCHAR str[256];
	TCHAR vs[256], ps[256];
	UINT num;
	UINT semantics = 0;

	tifstream f(i_shaderPath.c_str());

	if (f.fail())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	f >> vs >> ps;

	{
		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_POSITION << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_COLOR << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_NORMAL << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_TEXCOORD << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_BLENDINDEXCES << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_BLENDWEIGHT << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_POSITIONT << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_PSIZE << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_TANGENT << num);

		f >> str >> str >> num;
		while (num--) semantics |= (FE_SHADER_SEMANTIC_BINORMAL << num);
	}

	pShader = FEShader::CreateShader((i_shaderPath + vs).c_str(), (i_shaderPath + ps).c_str(), static_cast<FE_SHADER_SEMANTICS>(semantics));

	if (pShader == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	while (!f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Name")))
		{
			f >> str >> pShader->m_Name;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("countTexture")))
		{
			f >> str >> pShader->_countTexture;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("countMatrix")))
		{
			f >> str >> pShader->_countMatrix;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("countVector")))
		{
			f >> str >> pShader->_countVector;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("countScalar")))
		{
			f >> str >> pShader->_countScalar;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("useLight")))
		{
			f >> str >> pShader->_useLight;
		}
	}

	f.close();
}
void LoadMaterial(tstring i_mtrlPath)
{

	FEMaterial* pMaterial;
	TCHAR str[256];

	tifstream f(i_mtrlPath.c_str());

	if (f.fail())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	while (!f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Shader")))
		{
			f >> str >> str;

			pMaterial = FEMaterial::CreateMaterial(FEShader::Find(str));

			if (pMaterial == nullptr)
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
				return;
			}
		}

		else if (TCSCMP_SAME(str, FE_TEXT("Name")))
		{
			f >> str >> pMaterial->m_Name;
		}
	}

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

/*void CheckFiles(tstring i_path)
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
			LoadShader(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("extension")))
		{

		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
}*/

void FEFileManager::ImportFile(tstring i_fileName)
{
	tstring extension;
	FileNameExtension(i_fileName, extension);

	if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fes")))
	{
		LoadShader(i_fileName);
	}

	else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fem")))
	{
		LoadMaterial(i_fileName);
	}
}