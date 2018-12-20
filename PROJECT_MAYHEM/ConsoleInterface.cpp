#include "ConsoleInterface.h"

CONSOLE_SCREEN_BUFFER_INFO ConsoleInterface::conInfo;

void ConsoleInterface::Init() {
	AllocConsole();

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
	conInfo.dwSize.Y = 100;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
	HWND console = GetConsoleWindow();
	SetWindowPos(console, 0, 2000, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stdout);

	HelloMsg();
}

void ConsoleInterface::HelloMsg() {
	std::cout << "PROJECT MAYHEM TEST CONSOLE\n";
}
