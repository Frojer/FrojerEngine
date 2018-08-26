#pragma once

#include <FEDefine.h>
#include <FEMesh.h>
#include <FEShader.h>
#include <FEMaterial.h>

namespace FEFileManager
{
	void ConvertAllFileInPath(tstring i_filePath, tstring i_outPath);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEMesh* i_pMtrl);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEShader* i_pShader);
	void ExportFile(tstring i_filePath, tstring i_outPath, const FEMaterial* i_pMtrl);
}