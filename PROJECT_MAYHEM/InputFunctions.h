#pragma once
#include "DIRECTINPUT.h"
#include "PacketManager.h"

struct InputFunc {
	int keyCode;

	PacketData* pData;

	virtual Packet* Process() = 0;

	PacketData* GetPacketData() { return pData; }
	int GetKeyCode() const { return keyCode; }
	void SetKeyCode(int inputKeyCode) { keyCode = inputKeyCode; }

	InputFunc() :keyCode(0), pData(nullptr) {}
	InputFunc(int inputKeyCode) :keyCode(inputKeyCode), pData(nullptr) {}

	virtual ~InputFunc() { }
};

struct Forward :public InputFunc {
	Forward() :InputFunc(DIK_W) {}

	Packet* Process() {
		Packet* p = new Packet(0, 0, 1);
		return p;
	}
};

struct Backward : public InputFunc {
	Backward() :InputFunc(DIK_S) {}

	Packet* Process() {
		Packet* p = new Packet(0, 0, 2);
		return p;
	}
};

struct Left : public InputFunc {
	Left() :InputFunc(DIK_A) {}

	Packet* Process() {
		Packet* p = new Packet(0, 0, 3);
		return p;
	}
};

struct Right : public InputFunc {
	Right() :InputFunc(DIK_D) {}

	Packet* Process() {
		Packet* p = new Packet(0, 0, 4);
		return p;
	}
};

//--------------------------------------------
//--------------------LOOK--------------------
//--------------------------------------------

struct LookUp :public InputFunc {
	LookUp() :InputFunc(DIK_MOUSE_UP) {}

	Packet* Process() {
		Packet* p = new Packet(0, 1, 1);
		p->LoadData(pData);
		return p;
	}
};

struct LookDown :public InputFunc {
	LookDown() :InputFunc(DIK_MOUSE_DOWN) {}

	Packet* Process() {
		Packet* p = new Packet(0, 1, 2);
		p->LoadData(pData);
		return p;
	}
};

struct LookLeft :public InputFunc {
	LookLeft() :InputFunc(DIK_MOUSE_LEFT) {}

	Packet* Process() {
		Packet* p = new Packet(0, 1, 3);
		p->LoadData(pData);
		return p;
	}
};

struct LookRight :public InputFunc {
	LookRight() :InputFunc(DIK_MOUSE_RIGHT) {}

	Packet* Process() {
		Packet* p = new Packet(0, 1, 4);
		p->LoadData(pData);
		return p;
	}
};
