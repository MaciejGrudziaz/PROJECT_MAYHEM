#pragma once
#include "BasicCharacter.h"

class Player :public BasicCharacter, public AutoList<Player> {

public:
	Player() :BasicCharacter() {}
	Player(const char* name) :BasicCharacter(name) {}

};
