#pragma once
#include <vector>
#include <queue>
#include "Packet.h"

class PacketManager {
	//std::vector<Packet*> packets;
	std::queue<Packet*> packets;

public:
	void LoadPacket(Packet* packet) { packets.push(packet); }

	//Packet* PeekPacket(int index) { 
	//	if (index>=0 && index<packets.size())
	//		return packets[index];
	//	else return nullptr;
	//}

	Packet* GetPacket() {
		if (packets.size() > 0) {
			Packet* ret = packets.front();
			packets.pop();
			return ret;
		}
		else return nullptr;
	}

	void ClearAll() {
		while(!packets.empty())
			packets.pop();
	}

	void Pop() {
		if(packets.size() > 0)
			packets.pop();
	}

	//int Clear(int index) {
	//	if (index >= 0 && index < packets.size()) {
	//		int i = 0;
	//		for (std::vector<Packet*>::iterator it = packets.begin(); it != packets.end(); ++it) {
	//			if (i == index) {
	//				packets.erase(it);
	//				return 1;
	//			}
	//		}
	//	}
	//	
	//	return 0;
	//}

	bool PacketsAvailable()const { if (packets.empty()) return false; else return true; }

	int PacketsCount()const { return packets.size(); }
};