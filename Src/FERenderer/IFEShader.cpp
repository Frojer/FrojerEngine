#include "IFEShader.h"
#ifdef FE_DX11
#include "FEDX11Shader.h"
#endif

IFEShader* IFEShader::CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semanticsFlag)
{
	IFEShader* pShader = nullptr;

#ifdef FE_DX11
	pShader = new FEDX11Shader;
#endif

	if (pShader == nullptr) return nullptr;

	pShader->Create(i_vsName, i_psName, i_semanticsFlag);

	return pShader;
}