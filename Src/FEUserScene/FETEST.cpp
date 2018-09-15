#include "FETEST.h"
#include <FETime.h>
#include <FEMath.h>
#include <windows.h>
#include <IFEScene.h>
#include <FEDebug.h>


FETEST::FETEST()
{
}


FETEST::~FETEST()
{
}

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

void FETEST::Update()
{
	auto pDebug = FEDebug::GetInstance();
	///*
#define GRID_SIZE 50.0f
#define GRID_WIDTH 1.0f
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		// x축 그리드
		pDebug->DrawLine(FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector3(0, GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector4(0.3f, 0.3f, 0.3f, 1));
		pDebug->DrawLine(FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2)), FEVector3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2)), FEVector4(0.3f, 0.3f, 0.3f, 1));

		// y축 그리드
		pDebug->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2), 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector3(GRID_SIZE * GRID_WIDTH / 2, 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), FEVector4(0.3f, 0.3f, 0.3f, 1));
		pDebug->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, -(GRID_SIZE * GRID_WIDTH / 2)), FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, (GRID_SIZE * GRID_WIDTH / 2)), FEVector4(0.3f, 0.3f, 0.3f, 1));

		// z축 그리드
		pDebug->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), FEVector3(GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), FEVector4(0.3f, 0.3f, 0.3f, 1));
		pDebug->DrawLine(FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2), 0), FEVector3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2), 0), FEVector4(0.3f, 0.3f, 0.3f, 1));
	}
	//*/

	// 이동
	if (IsKeyDown('W'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, 1.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('S'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, -1.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('A'))		GetMyObject()->GetTransform()->Translate(FEVector3(-1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('D'))		GetMyObject()->GetTransform()->Translate(FEVector3(1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('Q'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 1.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('E'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, -1.0f, 0.0f) * FETime::GetDeltaTime());

	// 스케일
	if (IsKeyDown('Z'))
		GetMyObject()->GetTransform()->m_vScale.x += FETime::GetDeltaTime() * 10.0f;
	if (IsKeyDown('X'))		GetMyObject()->GetTransform()->m_vScale.x -= FETime::GetDeltaTime() * 10.0f;

	// 회전
	if (IsKeyDown('I'))		GetMyObject()->GetTransform()->Rotate(FEVector3(1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime()	 * 10.0f);
	if (IsKeyDown('K'))		GetMyObject()->GetTransform()->Rotate(FEVector3(-1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime()	 * 10.0f);
	if (IsKeyDown('J'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, -1.0f, 0.0f) * FETime::GetDeltaTime()	 * 10.0f);
	if (IsKeyDown('L'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 1.0f, 0.0f) * FETime::GetDeltaTime()	 * 10.0f);
	if (IsKeyDown('U'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, 0.1f) * FETime::GetDeltaTime()	 * 10.0f);
	if (IsKeyDown('O'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, -0.1f) * FETime::GetDeltaTime()	 * 10.0f);

	if (IsKeyUp(VK_SPACE))
	{
		//IFEScene::s_BGColor = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
		
		GetMyObject()->GetRenderer()->SetWireFrame(GetMyObject()->GetRenderer()->GetSolidFrame());
	}

	pDebug->DrawLine(FEVector3::Zero, GetMyObject()->GetTransform()->GetPositionWorld(), FEVector4(0.0f, 1.0, 0.0f, 1.0f));
}