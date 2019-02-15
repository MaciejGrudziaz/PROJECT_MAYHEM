#pragma once
#include <glm/vec3.hpp>
#include <vector>

struct PacketData {
	template<class T>
	T* GetData() { return dynamic_cast<T*>(this); }

	virtual ~PacketData() {}
};

struct Packet {
	int class_;
	int group_;
	int id_;

	PacketData* data;

	void LoadData(PacketData* inData) { data = inData; }

	template<class T>
	T* GetData() { if (data != nullptr) return data->GetData<T>(); else return nullptr; }

	Packet() :class_(0), group_(0), id_(0), data(nullptr) {}
	Packet(int pClass, int pGroup, int pID) :class_(pClass), group_(pGroup), id_(pID), data(nullptr) { }
	Packet(int pClass, int pGroup, int pID, PacketData* pData) :class_(pClass), group_(pGroup), id_(pID), data(pData) {}

	virtual ~Packet() {
		if(data!=nullptr)
			delete data;
	}
};

/*
class:
	-> 0 [input module actions]:
		group:
			-> 0 [move functions]
			-> 1 [look functions]
	-> 1 [collision detection]
		group:
			-> 0 [standard collision]
			-> 1 [collision with potential ground]
*/

//-------------------------------------------------------------------
//------------------------------PACKETS------------------------------
//-------------------------------------------------------------------

//struct MouseMoveData :public PacketData {
//	int moveVal;
//
//	MouseMoveData(int moveVal_) :moveVal(moveVal_) {}
//};

template<class T>
struct SingleValData:public PacketData{
	T val;

	SingleValData(T val_) :val(val_) {}
};

struct CollisionData : public PacketData {
	std::vector<glm::vec3> normals;

	~CollisionData() { normals.clear(); }
};

//struct GroundCollision :public PacketData {
//	float y;
//
//	GroundCollision(float mapPos_y) : y(mapPos_y){}
//};