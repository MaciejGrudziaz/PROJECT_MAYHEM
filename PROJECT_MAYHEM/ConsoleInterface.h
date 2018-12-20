#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

class ConsoleInterface {
	ConsoleInterface() {}
	ConsoleInterface(const ConsoleInterface&) {}
	ConsoleInterface& operator=(const ConsoleInterface&) {}

	static CONSOLE_SCREEN_BUFFER_INFO conInfo;

	static void HelloMsg();

public:

	static void Init();

};