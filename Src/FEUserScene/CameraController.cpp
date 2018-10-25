#include "CameraController.h"
#include <FETime.h>
#include <windows.h>

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

void CameraController::Update()
{
	// 이동
	if (IsKeyDown('W'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown('S'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * -15.0f);
	if (IsKeyDown('A'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * -15.0f);
	if (IsKeyDown('D'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown('Q'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown('E'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * -15.0f);

	// 회전
	if (IsKeyDown('I'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(-50.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('K'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(50.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('J'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, -50.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('L'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 50.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('U'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, 50.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('O'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, -50.0f) * FETime::GetDeltaTime());
}