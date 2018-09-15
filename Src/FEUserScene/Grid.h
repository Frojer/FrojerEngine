#pragma once

#include <FEObjectHeader.h>

class Grid : public FEBehaviour
{
public:
	Grid();
	virtual ~Grid();

	void Update() override;
};