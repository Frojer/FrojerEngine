#include "IFERenderer.h"
#include "FEDXRenderer.h"
//#include "FEGLRenderer.h"

IFERenderer::IFERenderer()
{
}


IFERenderer::~IFERenderer()
{
}


IFERenderer* IFERenderer::CreateRenderer(void* i_phWnd)
{
	IFERenderer* pRenderer = nullptr;
#ifdef _WIN32
	pRenderer = new FEDXRenderer;

	if (pRenderer == nullptr) return nullptr;
#else
#error �����찡 �ƴϾ߾�
	//pRenderer = new FEGLRenderer;

	if (pRenderer == nullptr) return nullptr;
#endif

	pRenderer->Create(i_phWnd);

	return pRenderer;
}