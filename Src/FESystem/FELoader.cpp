#include "FELoader.h"

#include <FEUtility.h>

#ifdef _WIN32
//-------------------------------
// ������ ���� �� ���� ã��
//-------------------------------
#include <io.h>
#include <conio.h>
//-------------------------------
#else
#endif

#define BUFFER_SIZE 1024

float FELoader::animationTime = 0.0f;
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

	// ID ���
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

	// ���̴� ����
	f >> vs >> ps;
	// ���̴� UUID
	f >> str >> str >> uuid;

	pShader = new FEShader(uuid);
	if (pShader == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	// ���̴� Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pShader->m_Name = StripQuotes(str);

	// ���̴� �ø�ƽ��
	f >> str >> str >> semantics;
	f >> str >> str >> pShader->_countTexture;
	f >> str >> str >> pShader->_countMatrix;
	f >> str >> str >> pShader->_countVector;
	f >> str >> str >> pShader->_countScalar;

	result = pShader->CreateShader((i_shaderPath + vs).c_str(), (i_shaderPath + ps).c_str(), static_cast<FE_SHADER_SEMANTICS>(semantics));

	if (!result) return;

	f.close();
}
void FELoader::LoadMaterial(tstring i_mtrlPath, tstring i_name)
{
	FEMaterial* pMaterial = nullptr;
	TCHAR str[BUFFER_SIZE];
	UINT i = 0;
	float x, y;
	INT64 uuid, shaderUUID;

	tifstream f(i_mtrlPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	// ID ���
	f >> str >> str >> uuid;
	// ���̴� ������
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
			f >> str >> str >> x;
			pMaterial->SetTextureAmount(i, x);
			// MapID
			f >> str >> str >> uuid;
			pMaterial->SetTexture(i, FETexture::Find(uuid));
			// Offset
			f >> str >> x >> y;
			pMaterial->SetTextureOffset(i, FEVector2(x, y));
			// Tiling
			f >> str >> x >> y;
			pMaterial->SetTextureTiling(i, FEVector2(x, y));
			// Angle
			f >> str >> x;
			pMaterial->SetTextureAngle(i, FEVector3(0.0f, 0.0f, x));
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
	float x, y;
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
			f >> str >> str >> x;
			pMaterial->SetTextureAmount(i, x);
			// MapID
			f >> str >> str >> uuid;
			pMaterial->SetTexture(i, FETexture::Find(uuid));
			// Offset
			f >> str >> x >> y;
			pMaterial->SetTextureOffset(i, FEVector2(x, y));
			// Tiling
			f >> str >> x >> y;
			pMaterial->SetTextureTiling(i, FEVector2(x, y));
			// Angle
			f >> str >> x;
			pMaterial->SetTextureAngle(i, FEVector3(0.0f, 0.0f, x));
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
	FEVector4 v;
	UINT vc, ic, vf;
	bool useAnim;
	FEMesh* pMesh = nullptr;
	FEGameObject* pObj;

	// '{'
	f >> str;

	// Object ID
	f >> str >> str >> uuid;
	pObj = new FEGameObject(uuid);
	pObj->_isPrefab = true;

	pObj->_pParent = pParent;
	if (pParent != nullptr)		pParent->_children[uuid] = pObj;

	// Mesh ID
	f >> str >> str >> meshID;

	// Object/Mesh Name
	f >> str >> str;
	f.getline(str, BUFFER_SIZE);
	pObj->m_Name = StripQuotes(str);

	// Material ID
	f >> str >> str >> mtrlID;

	// Transform Info
	// Position
	f >> str >> str >> v.x >> v.y >> v.z;
	pObj->GetTransform()->SetPositionLocal(v);
	// Rotation
	f >> str >> str >> v.x >> v.y >> v.z >> v.w;
	pObj->GetTransform()->SetRotationQuaternion(v);
	// Scale
	f >> str >> str >> v.x >> v.y >> v.z;
	pObj->GetTransform()->m_vScale = v;
	f >> str >> str >> v.x >> v.y >> v.z >> v.w;
	//pObj->GetTransform()->_qScaleRot = v;

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

		// �ε��� ���� ���
		for (UINT i = 0; i < ic; i++)
			f >> pMesh->m_indics[i].a >> pMesh->m_indics[i].b >> pMesh->m_indics[i].c;

		pMesh->UpdateMeshData();

		f >> str >> str >> useAnim;
		if (useAnim)
		{
			FEAnimation* pAnim = pObj->AddComponent<FEAnimation>();
			pAnim->m_maxAnimTime = animationTime;
			UINT apc, arc, asc;
			f >> str >> str >> apc;
			pAnim->m_animPos.resize(apc);
			for (int i = 0; i < apc; i++)		f >> pAnim->m_animPos[i].animTime >> pAnim->m_animPos[i].pos.x >> pAnim->m_animPos[i].pos.y >> pAnim->m_animPos[i].pos.z;
			f >> str >> str >> arc;
			pAnim->m_animRot.resize(arc);
			for (int i = 0; i < arc; i++)		f >> pAnim->m_animRot[i].animTime >> pAnim->m_animRot[i].qRot.x >> pAnim->m_animRot[i].qRot.y >> pAnim->m_animRot[i].qRot.z >> pAnim->m_animRot[i].qRot.w;
			f >> str >> str >> asc;
			pAnim->m_animScale.resize(asc);
			for (int i = 0; i < asc; i++)		f >> pAnim->m_animScale[i].animTime >> pAnim->m_animScale[i].scale.x >> pAnim->m_animScale[i].scale.y >> pAnim->m_animScale[i].scale.z >> pAnim->m_animScale[i].axis.x >> pAnim->m_animScale[i].axis.y >> pAnim->m_animScale[i].axis.z >> pAnim->m_animScale[i].angle;
		}

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

	f >> str >> str >> animationTime;

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

	// ���� ���� �� ��� ������ ã�´�.
	handle = _tfindfirst((i_filePath + FE_TEXT("*")).c_str(), &fd);

	// ������ �ϳ��� ���ٸ�
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

	// ���� ���� �� ��� ������ ã�´�.
	handle = _tfindfirst((i_filePath + FE_TEXT("*")).c_str(), &fd);

	// ������ �ϳ��� ���ٸ�
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

	// ���� ���� �� ��� ������ ã�´�.
	handle = _tfindfirst((i_filePath + FE_TEXT("*")).c_str(), &fd);

	// ������ �ϳ��� ���ٸ�
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

	// ���� ���� �� ��� ������ ã�´�.
	handle = _tfindfirst((i_filePath + FE_TEXT("*")).c_str(), &fd);

	// ������ �ϳ��� ���ٸ�
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