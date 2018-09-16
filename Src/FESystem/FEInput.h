#pragma once
#ifndef _FE_INPUT
#define _FE_INPUT

enum FE_KEYCODE
{
	FE_A
};

class FEInput
{
private:
	static FEInput* pInstance;

protected:
	FEInput();

	virtual bool Create() = 0;

public:
	~FEInput();

	static bool CreateInput();

	virtual void KeyUp(FE_KEYCODE key) = 0;
	virtual void KeyDown(FE_KEYCODE key) = 0;

	static FEInput* GetInstance();
};

#endif