//#include "FEDirectInput.h"
//#include "FESystem.h"
//#include "IFERenderer.h"
//#ifdef FE_DX11
//FEDirectInput::FEDirectInput()
//	: _pDirectInput(nullptr), _pKeyboard(nullptr), _pMouse(nullptr)
//{
//}
//FEDirectInput::~FEDirectInput()
//{
//}
//
//bool FEDirectInput::Create()
//{
//	//IFERenderer::GetInstance()->m_setting.uiWindowHeight;
//	//HINSTANCE* hInstnace = static_cast<HINSTANCE*>(FESystem::GetInstance()->_pInstanceHandle);
//	//HWND* hWnd = static_cast<HWND*>(FESystem::GetInstance()->_pWindow->GetWindowHandle());
//
//	//// Initialize the main direct input interface.
//	//auto result = DirectInput8Create(*hInstnace, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_pDirectInput, NULL);
//	//if(FAILED(result)) { return false; }
//	//// Initialize the direct input interface for the keyboard.
//	//result = _pDirectInput->CreateDevice(GUID_SysKeyboard, &_pKeyboard, NULL);
//	//if(FAILED(result)) { return false; }
//	//// Set the data format. In this case since it is a keyboard we can use the predefined data format.
//	//result = _pKeyboard->SetDataFormat(&c_dfDIKeyboard);
//	//if(FAILED(result)) { return false; }
//	//// Set the cooperative level of the keyboard to not share with other programs.
//	//result = _pKeyboard->SetCooperativeLevel(*hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//	//if(FAILED(result)) { return false; }
//	//// Now acquire the keyboard.
//	//result = _pKeyboard->Acquire();
//	//if(FAILED(result)) { return false; }
//	//// Initialize the direct input interface for the mouse.
//	//result = _pDirectInput->CreateDevice(GUID_SysMouse, &_pMouse, NULL);
//	//if(FAILED(result)) { return false; }
//	//// Set the data format for the mouse using the pre-defined mouse data format.
//	//result = _pMouse->SetDataFormat(&c_dfDIMouse);
//	//if(FAILED(result)) { return false; }
//	//// Set the cooperative level of the mouse to share with other programs.
//	//result = _pMouse->SetCooperativeLevel(*hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//	//if(FAILED(result)) { return false; }
//	//// Acquire the mouse.
//	//result = _pMouse->Acquire();
//	//if(FAILED(result)) { return false; } return true;
//}
//
//void FEDirectInput::KeyUp(FE_KEYCODE key)
//{
//
//}
//void FEDirectInput::KeyDown(FE_KEYCODE key)
//{
//
//}
//#endif