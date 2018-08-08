#pragma once
#ifndef _FE_COMPONENT
#define _FE_COMPONENT

#include "FEObjectHeader.h"

class FEObject;

class FEComponent : public IFEObject
{
private:
	size_t _typeID;
	UINT _typeSize;
	FEObject* _pObj;

private:
	FEComponent(const FEComponent& rhs) = delete;
	FEComponent& operator= (const FEComponent& rhs) = delete;

public:
	FEComponent() : _typeID(0), _typeSize(0), _pObj(nullptr) {}
	virtual ~FEComponent() = default;

	FEObject* GetMyObject() { return _pObj; }

protected:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void AfterUpdate() {};
	virtual void Render() {};

	friend class FEObject;
};

#endif