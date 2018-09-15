#pragma once
#ifndef _FE_LIGHT
#define _FE_LIGHT

#include "FEObjectHeader.h"

enum FE_LIGHT_TYPE : BYTE
{
	FE_LIGHT_TYPE_DIRECTION,
	FE_LIGHT_TYPE_POINT,
	FE_LIGHT_TYPE_SPOT
};

class FELight :	public FEBehaviour
{
private:
	static std::list<FELight*> _lightList;
public:
	FEVector4 m_diffuse;
	FEVector3 m_ambient;
	FEVector3 m_specular;
	float m_range;
	FE_LIGHT_TYPE m_lightType;

public:
	FELight();
	virtual ~FELight();

	friend class FEMaterial;
};

#endif