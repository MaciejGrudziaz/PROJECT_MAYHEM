#include "InputModule.h"

PacketManager InputModule::outPackets;
std::vector<InputFunc*> InputModule::inputFunctions;

void InputModule::Init() {
	DIRECTINPUT::Initialize();
	DIRECTINPUT::AcquireAll();

	inputFunctions.push_back(new Forward());
	inputFunctions.push_back(new Backward());
	inputFunctions.push_back(new Left());
	inputFunctions.push_back(new Right());
	inputFunctions.push_back(new LookUp());
	inputFunctions.push_back(new LookDown());
	inputFunctions.push_back(new LookLeft());
	inputFunctions.push_back(new LookRight());
}

void InputModule::Process() {
	DIRECTINPUT::Update();

	for (InputFunc* f : inputFunctions) {
		if (DIRECTINPUT::KeyDown(f->GetKeyCode())) {
			outPackets.LoadPacket(f->Process());
		}
		else {
			int val = DIRECTINPUT::GetMouseMovevementAction(f->GetKeyCode());
			if (val != 0) {
				f->pData = new MouseMoveData(val);
				outPackets.LoadPacket(f->Process());
			}
		}
	}

	if (AutoList<Player>::GetCount() > 0)
		SendPackets(AutoList<Player>::GetObj(0)->GetInputPacketManager());
}

void InputModule::End() {
	DIRECTINPUT::Shutdown();
}

void InputModule::SendPackets(PacketManager* destination) {
	while (outPackets.PacketsAvailable())
		destination->LoadPacket(outPackets.GetPacket());
}
