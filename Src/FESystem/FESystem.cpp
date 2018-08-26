#include "FESystem.h"
#include "FEFileManager.h"
#include <IFERenderer.h>
#include <FEDefine.h>
#include <FEUtility.h>
#include <FESceneManager.h>

#ifdef _WIN32
//-------------------------------
// ������ ���� �� ���� ã��
//-------------------------------
#include <io.h>
#include <conio.h>
//-------------------------------
#else
#endif

using namespace std;

#define BUFFER_SIZE 1024

FESystem* FESystem::_pInstance = nullptr;

FESystemSetting LoadSetting()
{
	FESystemSetting setting;

	TCHAR str[256];
	tifstream f(FE_TEXT("../../bin/SystemSetting.ini"));

	if (f.fail())
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

		if (_tcsnicmp(FE_TEXT("Width"), str, 5) == 0)
		{
			f >> str >> setting.uiWindowWidth;
		}

		else if (_tcsnicmp(FE_TEXT("Height"), str, 6) == 0)
		{
			f >> str >> setting.uiWindowHeight;
		}

		else if (_tcsnicmp(FE_TEXT("WindowMode"), str, 10) == 0)
		{
			f >> str >> setting.bWindowMode;
		}

		else if (_tcsnicmp(FE_TEXT("Borderless"), str, 10) == 0)
		{
			f >> str >> setting.bBorderless;
		}

		else if (_tcsnicmp(FE_TEXT("VSync"), str, 5) == 0)
		{
			f >> str >> setting.bVSync;
		}
	}

	f.close();

	return setting;
}
void LoadShader(tstring i_shaderPath, tstring i_name)
{
	bool result;
	FEShader* pShader;
	TCHAR str[BUFFER_SIZE];
	TCHAR vs[BUFFER_SIZE], ps[BUFFER_SIZE];
	UINT semantics = 0;

	tifstream f(i_shaderPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	f >> vs >> ps;
	f >> str >> str >> semantics;

	i_shaderPath = FE_TEXT("../../bin/Resource/shd/");

	pShader = new FEShader;
	if (pShader == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	result = pShader->CreateShader((i_shaderPath + vs).c_str(), (i_shaderPath + ps).c_str(), static_cast<FE_SHADER_SEMANTICS>(semantics));

	if (!result) return;

	pShader->m_Name = GetFileName(i_shaderPath + i_name);
	f >> str >> str >> pShader->_countTexture;
	f >> str >> str >> pShader->_countMatrix;
	f >> str >> str >> pShader->_countVector;
	f >> str >> str >> pShader->_countScalar;
	f >> str >> str >> pShader->_useLight;

	f.close();
}
void LoadMaterial(tstring i_mtrlPath, tstring i_name)
{
	FEMaterial* pMaterial = nullptr;
	TCHAR str[BUFFER_SIZE];

	tifstream f(i_mtrlPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	// ���̴� ������
	f >> str >> str >> str;

	pMaterial = new FEMaterial(FEShader::Find(str));

	if (pMaterial == nullptr)
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
		return;
	}

	pMaterial->m_Name = GetFileName(i_name);

	while (!f.eof())
	{
		f >> str;

		if (TCSCMP_SAME(str, FE_TEXT("Name")))
		{
			if (pMaterial == nullptr)
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
				return;
			}

			//f >> str >> pMaterial->m_Name;
		}
	}

	f.close();
}
void LoadMesh(tstring i_meshPath, tstring i_name)
{
	FEMesh* pMesh = new FEMesh();
	TCHAR uuid[33];
	TCHAR str[BUFFER_SIZE];
	UINT vc, ic, vf;

	tifstream f(i_meshPath + i_name);

	if (!f.is_open())
	{
		//FEDebug::WarningMessage(FE_TEXT("Failed to load Material."));
		return;
	}

	// �̸� ����
	pMesh->m_Name = GetFileName(i_name);

	// UUID ���
	f >> str >> str >> uuid;

	// ���ؽ� ���� ���
	f >> str >> str >> vf;

	// ���ؽ� ���� ���
	f >> str >> str >> vc;

#define VFCheck(format) (vf & format) == format
	pMesh->m_pos.resize(vc);
	if (VFCheck(FE_VF_VERTEX_COLOR))	pMesh->m_color.resize(vc);
	if (VFCheck(FE_VF_NORMAL))			pMesh->m_normal.resize(vc);
	if (VFCheck(FE_VF_TEXTURE))			pMesh->m_uv.resize(vc);

	// ���ؽ� ���� ���
	for (UINT i = 0; i < vc; i++)
	{
		f >> pMesh->m_pos[i].x >> pMesh->m_pos[i].y >> pMesh->m_pos[i].z;
		if (VFCheck(FE_VF_VERTEX_COLOR))	f >> pMesh->m_color[i].x >> pMesh->m_color[i].y >> pMesh->m_color[i].z >> pMesh->m_color[i].w;
		if (VFCheck(FE_VF_NORMAL))			f >> pMesh->m_normal[i].x >> pMesh->m_normal[i].y >> pMesh->m_normal[i].z;
		if (VFCheck(FE_VF_TEXTURE))			f >> pMesh->m_uv[i].x >> pMesh->m_uv[i].y;
	}
#undef VFCheck

	// �ε��� ���� ���
	f >> str >> str >> ic;
	pMesh->m_indics.resize(ic);

	// �ε��� ���� ���
	for (UINT i = 0; i < ic; i++)
		f >> pMesh->m_indics[i].a >> pMesh->m_indics[i].b >> pMesh->m_indics[i].c;

	pMesh->UpdateMeshData();

	f.close();
}
void FindShaderFile(tstring i_filePath)
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
void FindMaterialFile(tstring i_filePath)
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
void FindMeshFile(tstring i_filePath)
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

FESystem::FESystem()
	: _pWindow(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}

bool FESystem::Create(LPCTSTR i_sWindowName)
{
	//--------------
	// ���� �о����
	//--------------
	FESystemSetting setting = LoadSetting();


	//------------
	// ������ ����
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight, setting.bWindowMode, setting.bBorderless);

	if (_pWindow == nullptr)
		return false;


	//------------
	// ������ ����
	//------------
	IFERenderer::CreateRenderer(_pWindow->GetWindowHandle(), setting, FE_DX11);

	return true;
}


void FESystem::Release()
{
	delete FESceneManager::GetInstance();
	delete IFERenderer::GetInstance();

	SAFE_DELETE(_pWindow);
}


bool FESystem::LoadData()
{
	FESceneManager* pSM = FESceneManager::GetInstance();

	FEShader::CreateDefaultConstantBuffer();
	FindShaderFile(FE_TEXT("../../bin/Resource/"));
	FindMaterialFile(FE_TEXT("../../bin/Resource/"));
	FindMeshFile(FE_TEXT("../../bin/Resource/"));
	
	// ImportScene �Լ��� ���� ���� ���·� ������ ���� ������ �о� ������ �����ϵ��� ������
	// ȿ�� : FEScene Ŭ���� �ϳ��� ��� ������ ���� �� �ִ�. ������ ������ �ٸ� ���� ������Ʈ���� ��ġ �� �����̴�. �� �ٸ������� ������ �о� ���� ������ �� �ֵ��� ������.
	pSM->ImportScene();

	pSM->LoadScene((UINT)0);

	return true;
}


void FESystem::Run()
{
	FESceneManager* pSM = FESceneManager::GetInstance();
	IFERenderer* pRenderer = IFERenderer::GetInstance();

	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// �޼��� ����.
			break;

		pSM->ChangeScene();

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_BGColor);
		FESceneManager::_pCurrentScene->Render();
		pRenderer->Flip();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}