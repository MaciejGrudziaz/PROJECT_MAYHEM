#pragma once
#include <iostream>
#include "Character.h"
#include "Camera.h"

class BasicCharacter :public Character, public AutoList<BasicCharacter> {
	void ProcessPackets();

	void MoveAction(int dir1,int dir2);

	typedef std::map<const char*,int> AnimNameMap;
	AnimNameMap moveAnimNames;

	glm::vec3 prevPos, nextPos;
	glm::vec3 freeForceVec;

public:
	BasicCharacter() :Character(),freeForceVec(glm::vec3(0.0f)) {}

	BasicCharacter(const char* name) :Character(name) {}

	virtual void Init();

	virtual void Update();
};