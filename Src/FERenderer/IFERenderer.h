#pragma once
#ifndef _FE_IRENDERER
#define _FE_IRENDERER

#include <FEDefine.h>
#include <FEMath.h>
#include "FERendererDefine.h"
#include "IFEBuffer.h"

enum FE_Platform
{
	FE_DX11,
	//FE_GL,
};

struct FESystemSetting
{
	UINT uiWindowWidth, uiWindowHeight;

	// 전체화면 사용 여부
	bool bWindowMode;
	// 테두리 사용 여부
	bool bBorderless;
	// 수직동기화 사용 여부
	bool bVSync;
};

class IFERenderer
{
private:
	FE_Platform _platform;

public:
	FESystemSetting m_setting;

protected:
	virtual bool Create(void* i_phWnd) = 0;

private:
	static IFERenderer* _pInstance;

public:
	IFERenderer() = default;
	virtual ~IFERenderer() = default;

	virtual void SetVertexBuffers(UINT StartSlot, UINT NumBuffers, const IFEBuffer* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) const = 0;
	virtual void SetIndexBuffer(const IFEBuffer* pIndexBuffer, FEGI_FORMAT Format, UINT Offset) const = 0;
	virtual void SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const = 0;
	virtual void ClearBackBuffer(const FEVector4& i_color) const = 0;
	virtual void Flip() const = 0;

	FE_Platform GetPlatform() const;

	static IFERenderer* CreateRenderer(void* i_phWnd, const FESystemSetting& i_setting, const FE_Platform i_platform);
	static IFERenderer* GetInstance();
};

#endif