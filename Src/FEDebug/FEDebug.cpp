#include "FEDebug.h"
#include <FECamera.h>
#include <IFERenderer.h>

FEDebug* FEDebug::_pInstance = nullptr;
PointCB FEDebug::_PointCBData;

FEDebug* FEDebug::GetInstance()
{
	if (_pInstance == nullptr)
		_pInstance = new FEDebug;

	return _pInstance;
}

FEDebug::FEDebug()
{
}

FEDebug::~FEDebug()
{
}

bool FEDebug::Initialize()
{
	return true;
}
void FEDebug::Release()
{

}

void FEDebug::DrawLine(FEVector3 start, FEVector3 end, FEVector4 color)
{
#ifdef _DEBUG
	FEVector3 point[2];

	if (_pLineVB == nullptr)
	{
		_pLineVB = IFEBuffer::CreateBuffer(FE_BIND_VERTEX_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(point));
		_pLineCB = IFEBuffer::CreateBuffer(FE_BIND_CONSTANT_BUFFER, FE_USAGE_DYNAMIC, true, sizeof(PointCB));
		_pLineShader = IFEShader::CreateShader(FE_TEXT("Resource/Standard/Line.vso"), FE_TEXT("Resource/Standard/Line.pso"), FE_SHADER_SEMANTIC_POSITION);
	}

	// VB 업데이트
	point[0] = start;
	point[1] = end;
	_pLineVB->UpdateBuffer(point, sizeof(point));

	// Set VB
	UINT stride = sizeof(FEVector3);
	UINT offset = 0;
	IFERenderer::GetInstance()->SetVertexBuffer(0, 1, _pLineVB, &stride, &offset);

	// Set RenderState
	IFERenderer::GetInstance()->SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY_LINELIST);
	IFERenderer::GetInstance()->SetRSState(0);
	IFERenderer::GetInstance()->SetDSState(0, 0);

	// CB 업데이트
	_PointCBData.vColor = FEMath::FEConvertToAlignData(color);
	auto cams = FECamera::GetAllCameras();
	for (auto iter = cams.begin(); iter != cams.end(); iter++)
	{
		_PointCBData.mVP = FEMath::FEConvertToAlignData((*iter)->GetViewMatrixLH() * (*iter)->GetPerspectiveFovLH());
		_pLineCB->UpdateBuffer(&_PointCBData, sizeof(_PointCBData));

		// Set CB
		_pLineShader->SetConstantBuffer(0, _pLineCB);

		// Set Shader
		_pLineShader->Render();

		IFERenderer::GetInstance()->Draw(2, 0);
	}
#endif
}

void FEDebug::DrawNormal(FEGameObject* pObj, FEVector4 col)
{
#ifdef _DEBUG
	if (pObj->GetRenderer() != nullptr)
	{
		std::vector<FEVector3> pos = pObj->GetRenderer()->m_pMesh->m_pos;
		std::vector<FEVector3> nor = pObj->GetRenderer()->m_pMesh->m_normal;

		FEMatrix mWorld = pObj->GetTransform()->GetWorldMatrix();
		FEVector3 start, end;

		for (UINT i = 0; i < pos.size(); i++)
		{
			start = pos[i] * mWorld;
			//end = (pos[i] + nor[i]) * mWorld;
			end = (nor[i] * pObj->GetTransform()->GetRotationMatrix());
			end += (pos[i] * mWorld);

			DrawLine(start, end, col);
		}
	}

	auto children = pObj->GetChildren();

	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		DrawNormal(iter->second, col);
	}
#endif
}


void FEDebug::ErrorMessage(tstring text)
{

}

void FEDebug::WarningMessage(tstring text)
{

}