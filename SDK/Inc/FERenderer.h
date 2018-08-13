#pragma once
#ifndef _FE_RENDERER_COMPONENT
#define _FE_RENDERER_COMPONENT

#include "FEObjectHeader.h"

class FEMesh;
enum BLEND_STATE;

enum PRIMITIVE_TOPOLOGY
{
	PRIMITIVE_TOPOLOGY_POINTLIST,
	PRIMITIVE_TOPOLOGY_LINELIST,
	PRIMITIVE_TOPOLOGY_LINESTRIP,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
};

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

class FERenderer : public FEBehaviour
{
private:
	PRIMITIVE_TOPOLOGY _PT;
	BYTE _RSState;
	DWORD _DSState;
	//BLEND_STATE _BlendState;

public:
	UINT m_stencilRef;

	FEMesh* m_pMesh;
	FEMaterial* m_pMaterial;

private:
	virtual void Render() override;

public:
	FERenderer();
	virtual ~FERenderer();

	void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY i_pt);

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

	void SetBlendState(BLEND_STATE bs);
};

#endif