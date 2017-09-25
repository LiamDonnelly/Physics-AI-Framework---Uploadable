#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <Windows.h>
#include <dinput.h>

class InputManager
{
public:
	InputManager(HINSTANCE hinstance, HWND _hWnd);
	~InputManager();

	bool readKeyboard();
	bool readMouse();
	void update();
	void process();

	void getMouseLocation(int& mouseX, int& mouseY);
	bool isKeyPressed(unsigned char key);
private:

	HRESULT initInputDevices(HWND _hWnd);
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _currMouseState;

	float _mouseX;
	float _mouseY;

	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;
};

