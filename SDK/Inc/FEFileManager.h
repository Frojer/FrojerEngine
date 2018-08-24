#pragma once

#include <FEDefine.h>
#include <FEMesh.h>
#include <FEShader.h>
#include <FEMaterial.h>

namespace FEFileManager
{
	void ConvertAllFileInPath(tstring i_filePath);
	void ExportFile(tstring i_filePath, const FEMesh* i_pMtrl);
	void ExportFile(tstring i_filePath, const FEShader* i_pShader);
	void ExportFile(tstring i_filePath, const FEMaterial* i_pMtrl);
}