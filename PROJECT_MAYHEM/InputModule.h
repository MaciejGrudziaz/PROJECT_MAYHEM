#pragma once
#include "DIRECTINPUT.h"
#include "InputFunctions.h"
#include "Player.h"

class InputModule {
	static PacketManager outPackets;
	static std::vector<InputFunc*> inputFunctions;

	static void SendPackets(PacketManager* destination);

public:

	static void Init();

	static void Process();

	static void End();
};
