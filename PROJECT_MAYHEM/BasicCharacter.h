#pragma once
#include "Character.h"
#include "Camera.h"

class BasicCharacter :public Character, public AutoList<BasicCharacter> {
	void ProcessPackets();

	void MoveAction(int dir1,int dir2);

	typedef std::map<const char*,int> AnimNameMap;
	AnimNameMap moveAnimNames;

public:
	BasicCharacter() :Character() {}

	BasicCharacter(const char* name) :Character(name) {}

	void Init();

	void Update();
};