#include "IFERenderer.h"
#include "IFEShader.h"
#include "FEDX11Shader.h"

IFEShader* IFEShader::CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName)
{
	IFEShader* pShader = nullptr;

	switch (IFERenderer::GetInstance()->GetPlatform())
	{
	case FE_DX11:
		pShader = new FEDX11Shader;
	}

	if (pShader == nullptr) return nullptr;

	pShader->Create(i_vsName, i_psName);

	return pShader;
}