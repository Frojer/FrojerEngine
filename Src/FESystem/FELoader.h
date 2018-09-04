#pragma once
#ifndef _FE_LOADER
#define _FE_LOADER

#include <FEDefine.h>
#include <IFERenderer.h>
#include <FEObjectHeader.h>

class FELoader
{
public:
	static FESystemSetting LoadSetting();
	static void LoadShader(tstring i_shaderPath, tstring i_name);
	static void LoadMaterial(tstring i_mtrlPath, tstring i_name);
	static void LoadMaterialInMesh(tifstream &f);
	static void LoadMesh(tifstream &f, FEGameObject* pParent);
	static void LoadMesh(tstring i_meshPath, tstring i_name);
	static void FindShaderFile(tstring i_filePath);
	static void FindMaterialFile(tstring i_filePath);
	static void FindMeshFile(tstring i_filePath);
};

#endif