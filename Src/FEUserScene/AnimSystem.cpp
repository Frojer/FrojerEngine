#include "AnimSystem.h"
#include <FETime.h>
#include <windows.h>

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

void AnimSystem::Update()
{
	// 회전
	if (IsKeyDown('T'))		pLight->GetTransform()->Rotate(FEVector3(-50.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('G'))		pLight->GetTransform()->Rotate(FEVector3(50.0f, 0.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('F'))		pLight->GetTransform()->Rotate(FEVector3(0.0f, -50.0f, 0.0f) * FETime::GetDeltaTime());
	if (IsKeyDown('H'))		pLight->GetTransform()->Rotate(FEVector3(0.0f, 50.0f, 0.0f) * FETime::GetDeltaTime());


	// 이동
	if (IsKeyDown(VK_UP))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown(VK_DOWN))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetLookAt() * FETime::GetDeltaTime() * -15.0f);
	if (IsKeyDown(VK_LEFT))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * -15.0f);
	if (IsKeyDown(VK_RIGHT))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetRightVector() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown('Z'))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * 15.0f);
	if (IsKeyDown('X'))		Hero->GetTransform()->Translate(Hero->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * -15.0f);

}