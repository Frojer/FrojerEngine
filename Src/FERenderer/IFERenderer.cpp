#include "IFERenderer.h"
#include "FEDX11Renderer.h"
//#include "FEGLRenderer.h"

IFERenderer* IFERenderer::_pInstance = nullptr;

FE_Platform IFERenderer::GetPlatform() const
{
	return _platform;
}

IFERenderer* IFERenderer::CreateRenderer(void* i_phWnd, const FESystemSetting& i_setting, const FE_Platform i_platform)
{
	if (_pInstance != nullptr)
		return nullptr;

	switch (i_platform)
	{
	case FE_DX11:
		_pInstance = new FEDX11Renderer;
		break;
	//case FE_GL:
	//	pRenderer = new FEGLRenderer;
	//	break;
	}

	if (_pInstance == nullptr) return nullptr;

	_pInstance->m_setting = i_setting;
	_pInstance->Create(i_phWnd);

	return _pInstance;
}


IFERenderer* IFERenderer::GetInstance()
{
	return _pInstance;
}