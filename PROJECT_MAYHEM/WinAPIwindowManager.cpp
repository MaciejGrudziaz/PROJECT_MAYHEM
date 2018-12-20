#include "WinAPIwindowManager.h"

WindowsMap WinAPIwindowManager::windowsMap;
WinAPIwindow* WinAPIwindowManager::mainWindow;
MSG WinAPIwindowManager::msg = { 0 };
HINSTANCE WinAPIwindowManager::hInstance=nullptr;

int WinAPIwindowManager::RegisterWindow(WinAPIwindow* window) {
	if (window != nullptr) {
		std::pair<WindowsMap::iterator, bool> retVal;
		retVal = windowsMap.insert(WindowsMap::value_type(window->GetName(), window));
		window->LoadInstanceHandle(hInstance);
		if (retVal.second == true)
			window->SetupWindowSettings();

		if (window->GetHWND() == nullptr) {
			windowsMap.erase(retVal.first);
			return 0;
		}

		if (windowsMap.size() == 1) {
			mainWindow = window;
			wglMakeCurrent(window->GetHDC(), window->GetHGLRC());
			
			int error=glewInit();
			assert(!error);
		}

		return retVal.second;
	}

	return 0;
}

WinAPIwindow* WinAPIwindowManager::GetWindow(std::string name) {
	WindowsMap::iterator it;
	it = windowsMap.find(name);
	if (it == windowsMap.end())
		return nullptr;
	else return (*it).second;
}

int WinAPIwindowManager::SetCurrentWindowGraphics(std::string windowName) {
	WindowsMap::iterator it = windowsMap.find(windowName);
	if (it != windowsMap.end()) {
		wglMakeCurrent((*it).second->GetHDC(), (*it).second->GetHGLRC());
		glViewport(0, 0, (*it).second->GetScreenWidth(), (*it).second->GetScreenHeight());
		return 1;
	}
	else return 0;
}

int WinAPIwindowManager::SetMainWindow(std::string windowName) {
	WindowsMap::iterator it = windowsMap.find(windowName);
	if (it != windowsMap.end()) {
		mainWindow = (*it).second;
		return 1;
	}
	else return 0;
}

//przetwarzanie komunikatów systemu Windows
void WinAPIwindowManager::MessageProcessing() {
	TranslateMessage(&msg);							//przet³umaczenie wiadomoœci i przes³anie jej do systemu
	DispatchMessage(&msg);
}

void WinAPIwindowManager::SwapBuffers() {
	for (WindowsMap::iterator it = windowsMap.begin(); it != windowsMap.end(); ++it) {
		wglMakeCurrent((*it).second->GetHDC(), (*it).second->GetHGLRC());
		::SwapBuffers((*it).second->GetHDC());
	}
	wglMakeCurrent(mainWindow->GetHDC(), mainWindow->GetHGLRC());
}

//----------------------CALLBACK----------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE: {
		WinAPIwindow* window = (WinAPIwindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		window->CreateGraphicContext(hwnd);
		window->ShowWinAPIwindow();
		return 0;
	}
	case WM_CLOSE: {
		WindowsMap* winMaps = WinAPIwindowManager::GetAllWindows();
		for (WindowsMapIt it = winMaps->begin(); it != winMaps->end(); ++it) {
			if ((*it).second->GetHWND() == hwnd) {
				if (winMaps->size() == 1 || hwnd == WinAPIwindowManager::GetMainWindow()->GetHWND()) {
					PostQuitMessage(0);
					(*it).second->ReleaseGraphicContext();
				}
				else (*it).second->DeleteGraphicsContext();
				DestroyWindow((*it).second->GetHWND());
				winMaps->erase(it);
				break;
			}
		}
		return 0;
	}
	case WM_SIZE: {
		WindowsMap* winMaps = WinAPIwindowManager::GetAllWindows();
		for (WindowsMapIt it = winMaps->begin(); it != winMaps->end(); ++it) {
			if ((*it).second->GetHWND() == hwnd) {
				(*it).second->SetScreenSize(LOWORD(lParam), HIWORD(lParam));
				//(*it).second->SetupOpenGLScreenSettings();					//zwolnienie kontekstu grafiki aplikacji przez WinAPI
				(*it).second->UpdateProjectionMatrix();
				break;
			}
		}
		return 0;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_ESCAPE: {
			WindowsMap* map = WinAPIwindowManager::GetAllWindows();
			for (WindowsMap::iterator it = map->begin(); it != map->end(); ++it)
				DestroyWindow((*it).second->GetHWND());
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}
		return 0;
	}

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}