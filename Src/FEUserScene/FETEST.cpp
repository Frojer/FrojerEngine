#include "FETEST.h"
#include <FETime.h>
#include <FEMath.h>
#include <windows.h>
#include <IFEScene.h>


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
	// 이동
	if (IsKeyDown('W'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, 1.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('S'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, -1.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('A'))		GetMyObject()->GetTransform()->Translate(FEVector3(-1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('D'))		GetMyObject()->GetTransform()->Translate(FEVector3(1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('Q'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 1.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('E'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, -1.0f, 0.0f) * FETime::GetDeltaTime());

	// 회전
	if (IsKeyDown('I'))		GetMyObject()->GetTransform()->Rotate(FEVector3(1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('K'))		GetMyObject()->GetTransform()->Rotate(FEVector3(-1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('J'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, -1.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('L'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 1.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('U'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, 0.1f) * FETime::GetDeltaTime());
	if (IsKeyDown('O'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, -0.1f) * FETime::GetDeltaTime());

	if (IsKeyDown(VK_SPACE))
	{
		//IFEScene::s_BGColor = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
		
		GetMyObject()->GetRenderer()->SetWireFrame(GetMyObject()->GetRenderer()->GetSolidFrame());
	}
}