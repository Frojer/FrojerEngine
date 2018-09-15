#pragma once

#include <FEObjectHeader.h>

class FETEST : public FEBehaviour
{
public:
	FETEST();
	virtual ~FETEST();

	void Update() override;
};