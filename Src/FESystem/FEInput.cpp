#include "FEInput.h"
#include <FEBuildSetting.h>
#ifdef FE_DX11
#include "FEDirectInput.h"
#endif

FEInput* FEInput::pInstance = nullptr;

FEInput::FEInput()
{
}
FEInput::~FEInput()
{
}

bool FEInput::CreateInput()
{
#ifdef FE_DX11
	//pInstance = new FEDirectInput;
#endif

	if (pInstance == nullptr)
		return false;

	bool result = pInstance->Create();

	if (!result)
		delete pInstance;

	return result;
}

FEInput* FEInput::GetInstance()
{
	return pInstance;
}