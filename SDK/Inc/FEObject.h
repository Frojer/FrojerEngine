#pragma once
#ifndef _FE_OBJECT
#define _FE_OBJECT

#include "FEObjectHeader.h"

class FEObject : public IFEObject
{
public:
	tstring m_Name;

private:
	FEObject(const FEObject& rhs) = delete;
	FEObject& operator= (const FEObject& rhs) = delete;

public:
	FEObject();
	virtual ~FEObject();
};

#endif