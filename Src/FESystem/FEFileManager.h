#pragma once

#include <FEDefine.h>

class FEFileManager
{
private:
	FEFileManager() = delete;
public:
	~FEFileManager() = delete;

	void ImportFile(tstring i_fileName);
	void ExportFile();

	static FEFileManager* GetInstance();
};