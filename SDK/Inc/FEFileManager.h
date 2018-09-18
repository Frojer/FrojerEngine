#pragma once
#ifndef _FE_FILE_MANAGER
#define _FE_FILE_MANAGER

#include <FEDefine.h>
#include <FEMesh.h>
#include <FEShader.h>
#include <FEMaterial.h>

class FEFileManager
{
private:
	static FEFileManager* _pInstance;

	tstring _dataPath;
	tstring _outPath;

private:
	FEFileManager();

	void ConvertAllFileInPath(tstring i_filePath);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEMesh* i_pMtrl);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEShader* i_pShader);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEMaterial* i_pMtrl);
public:
	~FEFileManager();

	static FEFileManager* GetInstance();

	void Update();

#ifdef _UNICODE
	friend int wmain(int argc, wchar_t* argv[]);
#else
	friend void main(int argc, char* argv[]);
#endif
};
#endif