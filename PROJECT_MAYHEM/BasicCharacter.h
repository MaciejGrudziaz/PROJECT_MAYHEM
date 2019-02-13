#pragma once
#include "Character.h"
#include "Camera.h"

class BasicCharacter :public Character, public AutoList<BasicCharacter> {
	void ProcessPackets();

	void MoveAction(int dir1,int dir2);

	typedef std::map<const char*,int> AnimNameMap;
	AnimNameMap moveAnimNames;

	glm::vec3 prevPos, nextPos;

public:
	BasicCharacter() :Character() {}

	BasicCharacter(const char* name) :Character(name) {}

	virtual void Init();

	virtual void Update();
};