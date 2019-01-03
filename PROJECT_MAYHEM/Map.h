#pragma once
#include "Character.h"

class Map :public Character,public AutoList<Map> {


public:
	Map() :Character() {}
	Map(const char* name): Character(name) {}

};