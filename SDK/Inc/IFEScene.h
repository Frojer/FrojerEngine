#pragma once

#include <FEDefine.h>
#include <FEMath.h>

class IFEScene
{
public:
	tstring m_sName;
	static FEVector4 s_bkColor;

public:
	IFEScene() = default;
	virtual ~IFEScene() = default;

	virtual void Load() = 0;

	virtual void Initialize() = 0;

	virtual void Release() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
};