#include "FELight.h"

std::list<FELight*> FELight::_lightList;

FELight::FELight()
	: m_diffuse(FEVector4::One), m_ambient(FEVector3::One), m_specular(FEVector3::One), m_range(10.0f), m_lightType(FE_LIGHT_TYPE_POINT)
{
	_lightList.push_back(this);
}


FELight::~FELight()
{
	_lightList.remove(this);
}
