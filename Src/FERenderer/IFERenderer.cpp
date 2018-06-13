#include "IFERenderer.h"
#include "FEDXRenderer.h"
//#include "FEGLRenderer.h"

IFERenderer::IFERenderer()
{
}


IFERenderer::~IFERenderer()
{
}


IFERenderer* IFERenderer::CreateRenderer(void* i_phWnd, const FESystemSetting& i_Setting)
{
	IFERenderer* pRenderer = nullptr;
#ifdef _WIN32
	pRenderer = new FEDXRenderer;

	if (pRenderer == nullptr) return nullptr;
#else
#error 윈도우가 아니야아
	//pRenderer = new FEGLRenderer;

	if (pRenderer == nullptr) return nullptr;
#endif
	pRenderer->m_Setting = i_Setting;
	pRenderer->Create(i_phWnd);

	return pRenderer;
}