#include "IFERenderer.h"

#ifdef FE_DX11
#include "FEDX11Renderer.h"
#elif FE_GL
#include "FEGLRenderer.h"
#endif

IFERenderer* IFERenderer::_pInstance = nullptr;

IFERenderer* IFERenderer::CreateRenderer(void* i_phWnd, const FESystemSetting& i_setting)
{
	if (_pInstance != nullptr)
		return nullptr;

#ifdef FE_DX11
	_pInstance = new FEDX11Renderer;
#elif FE_GL
	_pInstance = new FEGLRenderer;
#endif

	if (_pInstance == nullptr) return nullptr;

	_pInstance->m_setting = i_setting;
	_pInstance->Create(i_phWnd);

	return _pInstance;
}


IFERenderer* IFERenderer::GetInstance()
{
	return _pInstance;
}