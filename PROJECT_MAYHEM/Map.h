#pragma once
#include "Character.h"

struct StaticObjectTransformedVert {
	std::vector<glm::vec3> vertices;

	glm::vec3 minVert, maxVert, midVert;

	~StaticObjectTransformedVert() {
		vertices.clear();
	}
};

struct Sector {
	glm::vec3 sectorCorners[4];
	glm::vec3 midVert;
	std::vector<glm::vec3> vertices;
};

class Map :public Character,public AutoList<Map> {
	//std::vector<Object*> ground;
	std::vector<int> groundObjIdx;
	std::vector<StaticObjectTransformedVert*> objectTransformedVertices;

	static const int sectorsCount = 32;

	Sector sectors[Map::sectorsCount];

public:
	Map() :Character() {}
	Map(const char* name): Character(name) {}

	bool SetGroundObject(const char* name);
	Object* GetGroundObject(int idx) { if (idx >= 0 && idx < groundObjIdx.size()) return GetModel()->GetObject_(groundObjIdx[idx]); else return nullptr; }
	int GetGroundObjIdx(int groundIdx) { if (groundIdx >= 0 && groundIdx < groundObjIdx.size()) return groundObjIdx[groundIdx]; else return -1; }
	int GetGroundCount()const { return groundObjIdx.size(); }
	void TransformObjectsVertices();

	StaticObjectTransformedVert* GetObjectTransformedVert(int idx) { if (idx >= 0 && idx < objectTransformedVertices.size()) return objectTransformedVertices[idx]; else return nullptr; }

	~Map() {
		for (int i = 0; i < objectTransformedVertices.size(); ++i)
			delete objectTransformedVertices[i];
	}
};
