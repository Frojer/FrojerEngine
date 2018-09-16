//#pragma once
//#ifndef _FE_DINPUT
//#define _FE_DINPUT
//#include <FEBuildSetting.h>
//#ifdef FE_DX11
//#include "FEInput.h"
//#include <FEDefine.h>
//
//#include <dinput.h>
//
//#define DIRECTINPUT_VERSION 0x0800
//#pragma once (lib, "dinput8.lib")
//#pragma once (lib, "dxguid.lib")
//
//
//class FEDirectInput : public FEInput
//{
//private:
//	IDirectInput8* _pDirectInput;
//	IDirectInputDevice8* _pKeyboard;
//	IDirectInputDevice8* _pMouse;
//	BYTE _keyboardState[256];
//	DIMOUSESTATE _mouseState;
//
//	int _mouseX, _mouseY;
//protected:
//	virtual bool Create() override;
//
//public:
//	FEDirectInput();
//	virtual ~FEDirectInput();
//
//	virtual void KeyUp(FE_KEYCODE key) override;
//	virtual void KeyDown(FE_KEYCODE key) override;
//};
//#endif
//#endif