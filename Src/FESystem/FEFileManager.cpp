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

	pShader = FEShader::CreateShader((i_shaderPath + vs).c_str(), (i_shaderPath + ps).c_str(), semantics);

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
void LoadMaterial(tstring i_path)
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
				LoadMaterial(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fem")))
		{
			FEMaterial* pMaterial;
			TCHAR str[256];

			tifstream f((i_path + fd.name).c_str());

			if (f.fail())
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
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

	}

	else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("feshader")))
	{

	}
}


void FEFileManager::ExportFile()
{

}


FEFileManager* FEFileManager::GetInstance()
{

}