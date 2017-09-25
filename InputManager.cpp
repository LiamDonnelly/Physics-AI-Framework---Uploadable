#include "InputManager.h"



InputManager::InputManager(HINSTANCE hinstance, HWND _hWnd)
{
	HRESULT hr = S_OK;
	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(0,0,L"Failed",0);
	}

	float _mouseX = 0.0f;
	float _mouseY = 0.0f;

	initInputDevices(_hWnd);
}


InputManager::~InputManager()
{
}

bool InputManager::readKeyboard()
{
	HRESULT hr;

	// Read the keyboard device.
	hr = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputManager::readMouse()
{
	HRESULT hr;

	hr = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_currMouseState);
	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputManager::update()
{
	
	bool result = readKeyboard();

	if (result)
	{
		result = readMouse();

		if (result)
		{
			process();
		}
	}
}

void InputManager::process()
{
	_mouseX += _currMouseState.lX;
	_mouseY += _currMouseState.lY;

	if (_mouseX < 0) { _mouseX = 0; }
	if (_mouseY < 0) { _mouseY = 0; }

	if (_mouseX > _renderWidth) { _mouseX = _renderWidth; }
	if (_mouseY > _renderHeight) { _mouseY = _renderHeight; }

}

void InputManager::getMouseLocation(int & mouseX, int & mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
}

bool InputManager::isKeyPressed(unsigned char key)
{
	if (_keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}


HRESULT InputManager::initInputDevices(HWND _hWnd)
{
	HRESULT hr = S_OK;

	hr = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_directInput->CreateDevice Failed", 0);
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	hr = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_keyboard->SetDataFormat Failed", 0);
	}

	hr = _keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_keyboard->SetCooperativeLevel Failed", 0);
	}

	// Now acquire the keyboard.
	hr = _keyboard->Acquire();
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_keyboard->Acquire Failed", 0);
	}

		// Initialize the direct input interface for the mouse.
	hr = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_directInput->CreateDevice Failed", 0);
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	hr = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L"_mouse->SetDataFormat Failed", 0);
	}
	

		// Set the cooperative level of the mouse to share with other programs.
	hr = _mouse->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(0, 0, L" _mouse->SetCooperativeLevel Failed", 0);
	}


		// Acquire the mouse.
	hr = _mouse->Acquire();
	if (FAILED(hr))
	{
		MessageBox(0, 0, L" _mouse->Acquire Failed", 0);
	}


	return hr;
}
