#include "FETEST.h"
#include <windows.h>


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
	if (IsKeyDown('W'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, 0.01f));
	if (IsKeyDown('S'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.0f, -0.01f));
	if (IsKeyDown('A'))		GetMyObject()->GetTransform()->Translate(FEVector3(-0.01f, 0.0f, 0.0f));
	if (IsKeyDown('D'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.01f, 0.0f, 0.0f));
	if (IsKeyDown('Q'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, 0.01f, 0.0f));
	if (IsKeyDown('E'))		GetMyObject()->GetTransform()->Translate(FEVector3(0.0f, -0.01f, 0.0f));

	// 회전
	if (IsKeyDown('I'))		GetMyObject()->GetTransform()->Rotate(FEVector3(1.0f, 0.0f, 0.0f));
	if (IsKeyDown('K'))		GetMyObject()->GetTransform()->Rotate(FEVector3(-1.0f, 0.0f, 0.0f));
	if (IsKeyDown('J'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, -1.0f, 0.0f));
	if (IsKeyDown('L'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 1.0f, 0.0f));
	if (IsKeyDown('U'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, 0.1f));
	if (IsKeyDown('O'))		GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, -0.1f));
}