#pragma once
#include "Character.h"

struct StaticObjectTransformedVert {
	std::vector<glm::vec3> vertices;

	glm::vec3 minVert, maxVert, midVert;
};

struct Sector {
	glm::vec3 sectorCorners[4];
	glm::vec3 midVert;
	std::vector<glm::vec3> vertices;
};

class Map :public Character,public AutoList<Map> {
	std::vector<Object*> ground;
	std::vector<StaticObjectTransformedVert*> objectTransformedVertices;

	static const int sectorsCount = 32;

	Sector sectors[Map::sectorsCount];

public:
	Map() :Character() {}
	Map(const char* name): Character(name) {}

	bool SetGroundObject(const char* name);
	void TransformObjectsVertices();


	~Map() {
		for (int i = 0; i < objectTransformedVertices.size(); ++i)
			delete objectTransformedVertices[i];
	}
};
