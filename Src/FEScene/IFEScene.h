#pragma once

#include <FEDefine.h>

class IFEScene
{
public:
	tstring m_sName;

public:
	IFEScene() = default;
	virtual ~IFEScene() = default;

	virtual void Load() = 0;

	virtual bool Create() = 0;
	virtual void Release() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
};