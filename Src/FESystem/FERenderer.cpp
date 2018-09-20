#include "FERenderer.h"
#include "FESceneManager.h"

FERenderer::FERenderer()
	: _RSState(0), _DSState(0), _RenderPriority(0), m_stencilRef(0), m_pMesh(nullptr), m_pMaterial(nullptr)
{

}

FERenderer::~FERenderer()
{

}


void FERenderer::Render()
{
	IFERenderer* pRenderer = IFERenderer::GetInstance();

	if (m_pMesh != nullptr && m_pMaterial != nullptr)
	{
		m_pMaterial->UpdateConstantBuffer(GetMyObject()->GetTransform());

		m_pMesh->Render();
		m_pMaterial->Render();

		pRenderer->SetRSState(_RSState);
		pRenderer->SetDSState(_DSState, m_stencilRef);
		pRenderer->SetBlendState(_BlendState);

		pRenderer->DrawIndexed(m_pMesh->m_indics.size() * 3, 0, 0);
	}
}


void FERenderer::SetWireFrame(bool i_bSet)
{
	_RSState &= 0x7F;
	_RSState |= i_bSet ? FE_RS_WIREFRAME : FE_RS_SOLID;
}
bool FERenderer::GetWireFrame()
{
	return (_RSState & 0x80) == FE_RS_WIREFRAME ? true : false;
}
void FERenderer::SetSolidFrame(bool i_bSet)
{
	SetWireFrame(!i_bSet);
}
bool FERenderer::GetSolidFrame()
{
	return !GetWireFrame();
}
void FERenderer::SetCullMode(CULL_MODE mode)
{
	_RSState &= 0x9F;
	_RSState |= mode;
}
CULL_MODE FERenderer::GetCullMode()
{
	return (CULL_MODE)(_RSState & 0x60);
}
void FERenderer::SetClockwise(bool i_bSet)
{
	_RSState &= 0xEF;
	_RSState |= i_bSet ? FE_RS_CLOCKWISE : FE_RS_COUNTER_CLOCKWISE;
}
bool FERenderer::GetClockwise()
{
	return (_RSState & 0x10) == FE_RS_CLOCKWISE;
}
void FERenderer::SetCounterClockwise(bool i_bSet)
{
	SetClockwise(!i_bSet);
}
bool FERenderer::GetCounterClockwise()
{
	return !GetClockwise();
}
void FERenderer::SetDepthClipEnable(bool i_bSet)
{
	_RSState &= 0xF7;
	_RSState |= i_bSet ? FE_RS_DEPTH_CLIP_ON : FE_RS_DEPTH_CLIP_OFF;
}
bool FERenderer::GetDepthClipEnable()
{
	return (_RSState & 0x08) == FE_RS_DEPTH_CLIP_ON ? true : false;
}
void FERenderer::SetScissorEnable(bool i_bSet)
{
	_RSState &= 0xFB;
	_RSState |= i_bSet ? FE_RS_SCISSOR_ON : FE_RS_SCISSOR_OFF;
}
bool FERenderer::GetScissorEnable()
{
	return (_RSState & 0x04) == FE_RS_SCISSOR_ON ? true : false;
}
void FERenderer::SetMultisampleEnable(bool i_bSet)
{
	_RSState &= 0xFD;
	_RSState |= i_bSet ? FE_RS_MULTISAMPLE_ON : FE_RS_MULTISAMPLE_OFF;
}
bool FERenderer::GetMultisampleEnable()
{
	return (_RSState & 0x02) == FE_RS_MULTISAMPLE_ON ? true : false;
}
void FERenderer::SetAntialiasedLineEnable(bool i_bSet)
{
	_RSState &= 0xFE;
	_RSState |= i_bSet ? FE_RS_ANTIALIASEDLINE_ON : FE_RS_ANTIALIASEDLINE_Off;
}
bool FERenderer::GetAntialiasedLineEnable()
{
	return (_RSState & 0x01) == FE_RS_ANTIALIASEDLINE_ON ? true : false;
}

void FERenderer::SetDepthEnable(bool enable)
{
	_DSState &= 0x7FFFFFFF;
	_DSState |= enable ? FE_DS_DEPTH_TEST_ON : FE_DS_DEPTH_TEST_OFF;
}
void FERenderer::SetDepthWrite(bool enable)
{
	_DSState &= 0xBFFFFFFF;
	_DSState |= enable ? FE_DS_DEPTH_WRITE_ON : FE_DS_DEPTH_WRITE_OFF;
}
void FERenderer::SetDepthFunc(COMPARISON_FUNC func)
{
	_DSState &= 0xC7FFFFFF;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= FE_DS_DEPTH_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= FE_DS_DEPTH_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= FE_DS_DEPTH_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= FE_DS_DEPTH_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= FE_DS_DEPTH_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= FE_DS_DEPTH_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= FE_DS_DEPTH_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= FE_DS_DEPTH_COMPARISON_ALWAYS;
		break;
	}
}
void FERenderer::SetStencilEnable(bool enable)
{
	_DSState &= 0xFBFFFFFF;
	_DSState |= enable ? FE_DS_STENCIL_ON : FE_DS_STENCIL_OFF;
}
void FERenderer::SetStencilFailOpFront(STENCIL_OP op)
{
	_DSState &= 0xFC7FFFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_FRONT_FAIL_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilDepthFailOpFront(STENCIL_OP op)
{
	_DSState &= 0xFF8FFFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilPassOpFront(STENCIL_OP op)
{
	_DSState &= 0xFFF1FFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_FRONT_PASS_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilFuncFront(COMPARISON_FUNC func)
{
	_DSState &= 0xFFFE3FFF;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= FE_DS_STENCIL_FRONT_COMPARISON_ALWAYS;
		break;
	}
}
void FERenderer::SetStencilFailOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFC7FF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_BACK_FAIL_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilDepthFailOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFF8FF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilPassOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFFF1F;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= FE_DS_STENCIL_BACK_PASS_OP_DECR;
		break;
	}
}
void FERenderer::SetStencilFuncBack(COMPARISON_FUNC func)
{
	_DSState &= 0xFFFFFFE3;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= FE_DS_STENCIL_BACK_COMPARISON_ALWAYS;
		break;
	}
}

void FERenderer::SetBlendState(const FE_BLEND_DESC& bs)
{
	_BlendState = bs;
}
FE_BLEND_DESC FERenderer::GetBlendState()
{
	return _BlendState;
}

void FERenderer::SetRenderPriority(const UINT i_priority)
{
	if (GetMyObject() == nullptr || _RenderPriority == i_priority)
		return;

	auto pScene = FESceneManager::GetCurrentScene();
	pScene->_renderMap[_RenderPriority].remove(this);
	_RenderPriority = i_priority;
	pScene->_renderMap[_RenderPriority].push_back(this);

	if (IFEScene::_maxPrioirty < i_priority)	IFEScene::_maxPrioirty = i_priority;
	else if (pScene->_renderMap[IFEScene::_maxPrioirty].begin() == pScene->_renderMap[IFEScene::_maxPrioirty].end())
	{
		do
		{
			--IFEScene::_maxPrioirty;
		} while (pScene->_renderMap[IFEScene::_maxPrioirty].begin() == pScene->_renderMap[IFEScene::_maxPrioirty].end());
	}
}