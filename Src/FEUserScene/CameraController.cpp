#include "CameraController.h"
#include <FETime.h>
#include <windows.h>

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

void CameraController::Update()
{
	// �̵�
	if (IsKeyDown('W'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * 5.0f);
	if (IsKeyDown('S'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * -5.0f);
	if (IsKeyDown('A'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * -5.0f);
	if (IsKeyDown('D'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * 5.0f);
	if (IsKeyDown('Q'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * 5.0f);
	if (IsKeyDown('E'))		pCam->GetMyObject()->GetTransform()->Translate(pCam->GetMyObject()->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * -5.0f);

	// ȸ��
	if (IsKeyDown('I'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(-1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime()	 * 50.0f);
	if (IsKeyDown('K'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(1.0f, 0.0f, 0.0f) * FETime::GetDeltaTime()	 * 50.0f);
	if (IsKeyDown('J'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, -1.0f, 0.0f) * FETime::GetDeltaTime()	 * 50.0f);
	if (IsKeyDown('L'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 1.0f, 0.0f) * FETime::GetDeltaTime()	 * 50.0f);
	if (IsKeyDown('U'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, 0.1f) * FETime::GetDeltaTime()	 * 50.0f);
	if (IsKeyDown('O'))		pCam->GetMyObject()->GetTransform()->Rotate(FEVector3(0.0f, 0.0f, -0.1f) * FETime::GetDeltaTime()	 * 50.0f);
}