#pragma once
#include <vector>
#include "Packet.h"

class PacketManager {
	std::vector<Packet*> packets;

public:
	void LoadPacket(Packet* packet) { packets.push_back(packet); }

	Packet* PeekPacket(int index) { 
		if (index>=0 && index<packets.size())
			return packets[index];
		else return nullptr;
	}

	Packet* GetPacket() {
		if (packets.size() > 0) {
			Packet* ret = packets.back();
			packets.pop_back();
			return ret;
		}
		else return nullptr;
	}

	void ClearAll() {
		packets.clear();
	}

	void Pop() {
		if(packets.size() > 0)
			packets.pop_back();
	}

	int Clear(int index) {
		if (index >= 0 && index < packets.size()) {
			int i = 0;
			for (std::vector<Packet*>::iterator it = packets.begin(); it != packets.end(); ++it) {
				if (i == index) {
					packets.erase(it);
					return 1;
				}
			}
		}
		
		return 0;
	}

	bool PacketsAvailable()const { if (packets.size() > 0) return true; else return false; }

	int PacketsCount()const { return packets.size(); }
};