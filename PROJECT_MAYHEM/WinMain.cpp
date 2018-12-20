#include <boost/thread.hpp>
#include "ConsoleInterface.h"
#include "ModulesManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsatnce, LPSTR lpCmdLine, int nShowCmd) {	
	WinAPIwindowManager::LoadInstanceHandle(hInstance);

	ModulesManager::Init();

	ConsoleInterface::Init();
	SetForegroundWindow(WinAPIwindowManager::GetMainWindow()->GetHWND());
	
	bool done = false;
	while (!done) {
		PeekMessage(WinAPIwindowManager::GetMSG(), NULL, 0,0,PM_REMOVE);

		if (WinAPIwindowManager::GetMSG()->message == WM_QUIT)
			done = true;
		else {
			WinAPIwindowManager::MessageProcessing();

			ModulesManager::Process();

			WinAPIwindowManager::SwapBuffers();
		}
	}

	ModulesManager::End();

	return (WinAPIwindowManager::GetMSG()->wParam);
}
