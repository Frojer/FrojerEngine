#pragma once
#ifndef _FE_IRENDERER
#define _FE_IRENDERER

#include <FEBuildSetting.h>
#include <FEDefine.h>
#include <FEMath.h>
#include "FERendererDefine.h"
#include "IFEBuffer.h"

struct FESystemSetting
{
	UINT uiWindowWidth, uiWindowHeight;

	// 전체화면 사용 여부
	bool bWindowMode;
	// 테두리 사용 여부
	bool bBorderless;
	// 수직동기화 사용 여부
	bool bVSync;


	UINT SampleCount;
	UINT SampleQuality;
	DWORD Anisotropy;
};

struct FEViewport
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};

class IFERenderer
{
public:
	FESystemSetting m_setting;

protected:
	virtual bool Create(void* i_phWnd) = 0;

private:
	static IFERenderer* _pInstance;

public:
	IFERenderer() = default;
	virtual ~IFERenderer() = default;

	virtual void SetRSState(BYTE flag) = 0;
	virtual void SetDSState(DWORD flag, UINT stencilRef) = 0;
	//virtual void SetBlendState(BLEND_STATE bs) = 0;

	virtual void SetViewports(UINT NumViewports, const FEViewport *pViewports) = 0;

	virtual void SetVertexBuffer(UINT StartSlot, UINT NumBuffers, const IFEBuffer* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) const = 0;
	virtual void SetIndexBuffer(const IFEBuffer* pIndexBuffer, FEGI_FORMAT Format, UINT Offset) const = 0;
	virtual void SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const = 0;
	
	virtual void ClearBackBuffer(const FEVector4& i_color) const = 0;
	virtual void Draw(UINT VertexCount, UINT StartVertexLocation) const = 0;
	virtual void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, UINT BaseVertexLocation) const = 0;
	virtual void Flip() const = 0;

	static IFERenderer* CreateRenderer(void* i_phWnd, const FESystemSetting& i_setting);
	static IFERenderer* GetInstance();
};

#endif