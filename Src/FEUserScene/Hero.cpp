#include "Hero.h"
#include <FETime.h>
#include <FEDebug.h>
#include <windows.h>

Hero::Hero()
{
}


Hero::~Hero()
{
}

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

void Hero::Update()
{
	// ÀÌµ¿
	if (IsKeyDown(VK_UP))		m_pHero->GetTransform()->Translate(m_pHero->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * -5.0f);
	if (IsKeyDown(VK_DOWN))		m_pHero->GetTransform()->Translate(m_pHero->GetTransform()->GetUpVector() * FETime::GetDeltaTime() * 5.0f);
	if (IsKeyDown(VK_LEFT))		m_pHero->GetTransform()->Rotate(FEVector3::Up, FEConvertToRadian(FETime::GetDeltaTime() * -90.0f));
	if (IsKeyDown(VK_RIGHT))	m_pHero->GetTransform()->Rotate(FEVector3::Up, FEConvertToRadian(FETime::GetDeltaTime() * 90.0f));

}