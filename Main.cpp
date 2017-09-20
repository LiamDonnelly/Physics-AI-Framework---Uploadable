#include "Application.h"

void gameLoopDelay(DWORD frameStartTime, float frameInterval)
{
	DWORD frameProcessingTime = GetTickCount() - frameStartTime;

	if (frameProcessingTime < frameInterval)
	{
		Sleep(frameInterval - frameProcessingTime);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Application * theApp = new Application();

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

	// Main message loop
	MSG msg = { 0 };

	float frameInterval = 25.0f;

	while (WM_QUIT != msg.message)
	{
		DWORD frameStartTime = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			bool handled = false;

			if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
			{
				handled = theApp->HandleKeyboard(msg);
			}
			else if (WM_QUIT == msg.message)
				break;

			if (!handled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			theApp->Update(frameInterval);
			theApp->Draw();

			gameLoopDelay(frameStartTime, frameInterval);
		}
	}

	delete theApp;
	theApp = nullptr;

	return (int)msg.wParam;
}