#pragma once

#include "FEObjectHeader.h"

class FEBehaviour : public FEComponent
{
private:
	BYTE _bEnable;

private:
	bool IsChanged();

public:
	FEBehaviour();
	virtual ~FEBehaviour();

	void SetEnable(bool enable);
	bool GetEnable();
};