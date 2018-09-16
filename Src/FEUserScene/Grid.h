#pragma once

#include <FEObjectHeader.h>

class Grid : public FEBehaviour
{
public:
	bool x, y, z;
public:
	Grid() = default;
	virtual ~Grid() = default;

	void Update() override;
};