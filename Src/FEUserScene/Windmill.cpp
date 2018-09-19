#include "Windmill.h"
#include <FETime.h>
#include <FEDebug.h>

Windmill::Windmill()
{
}


Windmill::~Windmill()
{
}


void Windmill::Update()
{
	auto pDebug = FEDebug::GetInstance();
	pDebug->DrawNormal(m_pWindmillBody, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));

	switch (m_type)
	{
	case WINDMILL_NORMAL:
		m_pWindmillWing[0]->GetTransform()->Rotate(FEVector3::Forward * -180.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		pDebug->DrawNormal(m_pWindmillWing[0], FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;

	case WINDMILL_TRIPLE_TYPE1:
		m_pWindmillWing[0]->GetTransform()->Rotate(FEVector3::Forward * 40.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		m_pWindmillWing[1]->GetTransform()->Rotate(FEVector3::Forward * -220.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		m_pWindmillWing[2]->GetTransform()->Rotate(FEVector3::Forward * 540.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		for (int i = 0; i < 3; i++)		pDebug->DrawNormal(m_pWindmillWing[i], FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;

	case WINDMILL_TRIPLE_TYPE2:
		m_pWindmillWing[0]->GetTransform()->Rotate(FEVector3::Forward * 40.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		m_pWindmillWing[1]->GetTransform()->Rotate(FEVector3::Forward * -180.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		m_pWindmillWing[2]->GetTransform()->Rotate(FEVector3::Forward * 360.0f * FETime::GetDeltaTime() * m_pSystem->m_cold);
		for (int i = 0; i < 3; i++)		pDebug->DrawNormal(m_pWindmillWing[i], FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		break;
	}
}