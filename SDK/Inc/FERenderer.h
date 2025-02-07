#pragma once
#ifndef _FE_RENDERER_COMPONENT
#define _FE_RENDERER_COMPONENT

#include "FEObjectHeader.h"
#include <IFERenderer.h>

class FEMesh;

enum CULL_MODE
{
	CULL_NONE = 0x00,
	CULL_FRONT = 0x40,
	CULL_BACK = 0x20,
};

enum STENCIL_OP
{
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INCR_SAT,
	STENCIL_OP_DECR_SAT,
	STENCIL_OP_INVERT,
	STENCIL_OP_INCR,
	STENCIL_OP_DECR
};

enum COMPARISON_FUNC
{
	COMPARISON_NEVER,
	COMPARISON_LESS,
	COMPARISON_EQUAL,
	COMPARISON_LESS_EQUAL,
	COMPARISON_GREATER,
	COMPARISON_NOT_EQUAL,
	COMPARISON_GREATER_EQUAL,
	COMPARISON_ALWAYS
};
enum RENDER_TYPE : BYTE
{
	RENDER_TYPE_NORMAL,
	RENDER_TYPE_TWOFACE,
};

class FERenderer : public FEBehaviour
{
private:
	BYTE _RSState;
	DWORD _DSState;
	FE_BLEND_DESC _BlendState;
	UINT _RenderPriority;

public:
	UINT m_stencilRef;
	RENDER_TYPE m_renderType;

	FEMesh* m_pMesh;
	FEMaterial* m_pMaterial;

private:
	virtual void Render() override;

public:
	FERenderer();
	virtual ~FERenderer();

	void		SetWireFrame(bool i_bSet);
	bool		GetWireFrame();
	void		SetSolidFrame(bool i_bSet);
	bool		GetSolidFrame();
	void		SetCullMode(CULL_MODE mode);
	CULL_MODE	GetCullMode();
	void		SetClockwise(bool i_bSet);
	bool		GetClockwise();
	void		SetCounterClockwise(bool i_bSet);
	bool		GetCounterClockwise();
	void		SetDepthClipEnable(bool i_bSet);
	bool		GetDepthClipEnable();
	void		SetScissorEnable(bool i_bSet);
	bool		GetScissorEnable();
	void		SetMultisampleEnable(bool i_bSet);
	bool		GetMultisampleEnable();
	void		SetAntialiasedLineEnable(bool i_bSet);
	bool		GetAntialiasedLineEnable();

	void SetDepthEnable(bool enable);
	void SetDepthWrite(bool enable);
	void SetDepthFunc(COMPARISON_FUNC func);
	void SetStencilEnable(bool enable);
	void SetStencilFailOpFront(STENCIL_OP op);
	void SetStencilDepthFailOpFront(STENCIL_OP op);
	void SetStencilPassOpFront(STENCIL_OP op);
	void SetStencilFuncFront(COMPARISON_FUNC func);
	void SetStencilFailOpBack(STENCIL_OP op);
	void SetStencilDepthFailOpBack(STENCIL_OP op);
	void SetStencilPassOpBack(STENCIL_OP op);
	void SetStencilFuncBack(COMPARISON_FUNC func);

	void SetBlendState(const FE_BLEND_DESC& bs);
	FE_BLEND_DESC GetBlendState();

	void SetRenderPriority(const UINT i_priority);

	friend class FEGameObject;
	friend class IFEScene;
};

#endif