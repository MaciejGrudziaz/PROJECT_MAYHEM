#pragma once

struct PacketData {
	virtual ~PacketData() {}
};

struct Packet {
	int class_;
	int group_;
	int id_;

	PacketData* data;

	void LoadData(PacketData* inData) { data = inData; }

	Packet() :class_(0), group_(0), id_(0), data(nullptr) {}
	Packet(int pClass, int pGroup, int pID) :class_(pClass), group_(pGroup), id_(pID), data(nullptr) { }

	virtual ~Packet() {
		if(data!=nullptr)
			delete data;
	}
};

//-------------------------------------------------------------------
//------------------------------PACKETS------------------------------
//-------------------------------------------------------------------

struct MouseMoveData :public PacketData {
	int moveVal;

	MouseMoveData(int moveVal_) :moveVal(moveVal_) {}
};