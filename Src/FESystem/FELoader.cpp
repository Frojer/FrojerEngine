#include "FELoader.h"

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

std::unordered_map<INT64, tstring> FELoader::resourceMap;

FESystemSetting FELoader::LoadSetting()
{
	FESystemSetting setting;

	TCHAR str[256];
	tifstream f(FE_TEXT("SystemSetting.ini"));

	if (!f.is_open())
	{
		setting.uiWindowWidth = 800;
		setting.uiWindowHeight = 600;
		setting.bWindowMode = false;
		setting.bBorderless = false;
		setting.bVSync = false;
	}

	while (!f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Width")))
		{
			f >> str >> setting.uiWindowWidth;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("Height")))
		{
			f >> str >> setting.uiWindowHeight;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("WindowMode")))
		{
			f >> str >> setting.bWindowMode;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("Borderless")))
		{
			f >> str >> setting.bBorderless;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("VSync")))
		{
			f >> str >> setting.bVSync;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("SampleCount")))
		{
			f >> str >> setting.SampleCount;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("SampleQuality")))
		{
			f >> str >> setting.SampleQuality;
		}

		else if (TCSCMP_SAME(str, FE_TEXT("Anisotropy")))
		{
			f >> str >> setting.Anisotropy;
		}
	}

	f.close();

	return setting;
}
void FELoader::LoadTexture(tstring i_texturePath, tstring i_name)
{
	TCHAR str[BUFFER_SIZE];
	FETexture* pTex;
	INT64 uuid;

	tifstream f(i_texturePath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	// ID 얻기
	f >> str >> str >> uuid;

	pTex = new FETexture(uuid);

	pTex->CreateTexture(i_texturePath + GetFileName(i_name));
}
void FELoader::LoadShader(tstring i_shaderPath, tstring i_name)
{
	bool result;
	FEShader* pShader;
	TCHAR str[BUFFER_SIZE];
	TCHAR vs[BUFFER_SIZE], ps[BUFFER_SIZE];
	UINT semantics = 0;
	INT64 uuid;

	tifstream f(i_shaderPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	// 셰이더 파일
	f >> vs >> ps;
	// 셰이더 UUID
	f >> str >> str >> uuid;

	pShader = new FEShader(uuid);
	if (pShader == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	// 셰이더 Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pShader->m_Name = StripQuotes(str);

	// 셰이더 시맨틱스
	f >> str >> str >> semantics;

	pShader->m_Name = GetFileName(i_shaderPath + i_name);
	f >> str >> str >> pShader->_countMatrix;
	f >> str >> str >> pShader->_countVector;
	f >> str >> str >> pShader->_countScalar;
	f >> str >> str >> pShader->_useLight;

	result = pShader->CreateShader((i_shaderPath + vs).c_str(), (i_shaderPath + ps).c_str(), static_cast<FE_SHADER_SEMANTICS>(semantics));

	if (!result) return;

	f.close();
}
void FELoader::LoadMaterial(tstring i_mtrlPath, tstring i_name)
{
	FEMaterial* pMaterial = nullptr;
	TCHAR str[BUFFER_SIZE];
	UINT i = 0;
	INT64 uuid, shaderUUID;

	tifstream f(i_mtrlPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	// ID 얻기
	f >> str >> str >> uuid;
	// 셰이더 얻어오기
	f >> str >> str >> shaderUUID;

	pMaterial = new FEMaterial(uuid, FEShader::Find(shaderUUID));

	if (pMaterial == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}
	// Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pMaterial->m_Name = StripQuotes(str);
	// Diffuse
	f >> str >> pMaterial->m_diffuse.x >> pMaterial->m_diffuse.y >> pMaterial->m_diffuse.z >> pMaterial->m_diffuse.w;
	// Ambient
	f >> str >> pMaterial->m_ambient.x >> pMaterial->m_ambient.y >> pMaterial->m_ambient.z;
	// Specular
	f >> str >> pMaterial->m_specular.x >> pMaterial->m_specular.y >> pMaterial->m_specular.z;
	// Power
	f >> str >> str >> pMaterial->m_power;

	do
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Map")))
		{
			// MapIndex >> '{'
			f >> i >> str;
			// Amount
			f >> str >> str >> i;
			// MapID
			f >> str >> str >> uuid;
			pMaterial->SetTexture(i, FETexture::Find(uuid));
			// Offset
			f >> str >> str >> str;
			// Tiling
			f >> str >> str >> str;
			// Angle
			f >> str >> str;
			// '}'
			f >> str;
		}
	} while (!f.eof());

	f.close();
}
void FELoader::LoadMaterialInMesh(tifstream &f)
{
	INT64 uuid;
	UINT i = 0;
	FEMaterial* pMaterial = nullptr;
	TCHAR str[BUFFER_SIZE];

	// '{'
	f >> str;
	// ID
	f >> str >> str >> uuid;
	pMaterial = new FEMaterial(uuid, FEShader::Find(FE_TEXT("Standard")));

	if (pMaterial == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	// Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pMaterial->m_Name = StripQuotes(str);
	// Diffuse
	f >> str >> pMaterial->m_diffuse.x >> pMaterial->m_diffuse.y >> pMaterial->m_diffuse.z >> pMaterial->m_diffuse.w;
	// Ambient
	f >> str >> pMaterial->m_ambient.x >> pMaterial->m_ambient.y >> pMaterial->m_ambient.z;
	// Specular
	f >> str >> pMaterial->m_specular.x >> pMaterial->m_specular.y >> pMaterial->m_specular.z;
	// Power
	f >> str >> str >> pMaterial->m_power;

	do
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Map")))
		{
			// MapIndex >> '{'
			f >> i >> str;
			// Amount
			f >> str >> str >> i;
			// MapID
			f >> str >> str >> uuid;
			pMaterial->SetTexture(i, FETexture::Find(uuid));
			// Offset
			f >> str >> str >> str;
			// Tiling
			f >> str >> str >> str;
			// Angle
			f >> str >> str;
			// '}'
			f >> str;
		}
	} while (str[0] != FE_TEXT('}'));
}
void FELoader::LoadMesh(tifstream &f, FEGameObject* pParent)
{
	INT64 uuid;
	TCHAR str[BUFFER_SIZE];
	INT64 mtrlID, meshID;
	UINT vc, ic, vf;
	FEMesh* pMesh = nullptr;
	FEGameObject* pObj;

	// '{'
	f >> str;

	// Object ID
	f >> str >> str >> uuid;
	pObj = new FEGameObject(uuid);

	pObj->_pParent = pParent;

	// Mesh ID
	f >> str >> str >> meshID;

	// Object/Mesh Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pObj->m_Name = StripQuotes(str);

	// Material ID
	f >> str >> str >> mtrlID;

	// Vertex Format
	f >> str >> str >> vf;
	if (meshID != 0)
	{
		pMesh = new FEMesh(meshID);
		pMesh->m_Name = pObj->m_Name;

		// Vertex Count
		f >> str >> str >> vc;

#define VFCheck(format) (vf & format) == format
		if (VFCheck(FE_VF_POSITION))		pMesh->m_pos.resize(vc);
		if (VFCheck(FE_VF_VERTEX_COLOR))	pMesh->m_color.resize(vc);
		if (VFCheck(FE_VF_NORMAL))			pMesh->m_normal.resize(vc);
		if (VFCheck(FE_VF_TEXTURE))			pMesh->m_uv.resize(vc);

		// Vertex Information
		for (UINT i = 0; i < vc; i++)
		{
			if (VFCheck(FE_VF_POSITION))		f >> pMesh->m_pos[i].x >> pMesh->m_pos[i].y >> pMesh->m_pos[i].z;
			if (VFCheck(FE_VF_VERTEX_COLOR))	f >> pMesh->m_color[i].x >> pMesh->m_color[i].y >> pMesh->m_color[i].z >> pMesh->m_color[i].w;
			if (VFCheck(FE_VF_NORMAL))			f >> pMesh->m_normal[i].x >> pMesh->m_normal[i].y >> pMesh->m_normal[i].z;
			if (VFCheck(FE_VF_TEXTURE))			f >> pMesh->m_uv[i].x >> pMesh->m_uv[i].y;
		}
#undef VFCheck

		// Index Count
		f >> str >> str >> ic;
		pMesh->m_indics.resize(ic);

		// 인덱스 정보 얻기
		for (UINT i = 0; i < ic; i++)
			f >> pMesh->m_indics[i].a >> pMesh->m_indics[i].b >> pMesh->m_indics[i].c;

		pMesh->UpdateMeshData();

		pObj->AddComponent<FERenderer>();
		if (mtrlID == 0)
		{
			pObj->GetRenderer()->m_pMaterial = new FEMaterial(FEShader::Find(FE_TEXT("Standard")));
			pObj->GetRenderer()->m_pMaterial->m_Name = FE_TEXT("New Material");
		}
		else	pObj->GetRenderer()->m_pMaterial = FEMaterial::Find(mtrlID);
		pObj->GetRenderer()->m_pMesh = pMesh;
	}

	do
	{
		f >> str;
		if (TCSCMP_SAME(str, FE_TEXT("MeshObject")))
		{
			LoadMesh(f, pObj);
		}
	} while (str[0] != FE_TEXT('}'));
}
void FELoader::LoadMesh(tstring i_meshPath, tstring i_name)
{
	TCHAR str[BUFFER_SIZE];

	tifstream f(i_meshPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	while (!f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Material")))
		{
			LoadMaterialInMesh(f);
		}

		else if (TCSCMP_SAME(str, FE_TEXT("MeshObject")))
		{
			LoadMesh(f, nullptr);
		}
	}

	f.close();
}
void FELoader::FindTextureFile(tstring i_filePath)
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
				FindTextureFile(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fet")))
		{
			LoadTexture(i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FELoader::FindShaderFile(tstring i_filePath)
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
				FindShaderFile(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fes")))
		{
			LoadShader(i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FELoader::FindMaterialFile(tstring i_filePath)
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
				FindMaterialFile(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fem")))
		{
			LoadMaterial(i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}
void FELoader::FindMeshFile(tstring i_filePath)
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
				FindMeshFile(i_filePath + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("msh")))
		{
			LoadMesh(i_filePath, fd.name);
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#endif
}