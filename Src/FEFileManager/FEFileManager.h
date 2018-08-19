#pragma once

#include <FEDefine.h>

namespace FEFileManager
{
	void ConvertAllFileInPath(tstring i_filePath);
	void ConvertShaderFile(tstring i_fileName);
	void ConvertASEMeshFile(tstring i_fileName);
	void ImportFile(tstring i_fileName);
	void ExportFile();
}