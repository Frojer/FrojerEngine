#include "FEObjectHeader.h"

FEBehaviour::FEBehaviour()
{
	//_type = COMPONENT_TYPE_UPDATE;
	_bEnable = 0x11;
}

FEBehaviour::~FEBehaviour()
{

}

bool FEBehaviour::IsChanged()
{
	if ((_bEnable & 0x01) != (_bEnable & 0x10) >> 4)
	{
		_bEnable |= (_bEnable & 0x01) << 4;
		return true;
	}

	return false;
}


void FEBehaviour::SetEnable(bool enable)
{
	if (enable)
		_bEnable |= 0x01;
	else
		_bEnable &= 0x10;
}


bool FEBehaviour::GetEnable()
{
	return (_bEnable & 0x01) == 0x01;
}