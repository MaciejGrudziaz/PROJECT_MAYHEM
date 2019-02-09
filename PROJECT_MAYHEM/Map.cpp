#include "Map.h"

bool Map::SetGroundObject(const char* name) {
	for (int i = 0; i < GetModel()->GetObjectsCount(); ++i) {
		if (GetModel()->GetObject_(i)->GetBasicObject()->name == name) {
			ground.push_back(GetModel()->GetObject_(i));
			return true;
		}
	}

	return false;
}

void Map::TransformObjectsVertices() {
	glm::mat4 modelTransform(1.0f);
	glm::mat4 objectGlobalTransform;
	glm::mat4 transformMat;
	glm::vec3 vert;

	modelTransform = glm::translate(modelTransform, GetPosition());
	modelTransform = modelTransform * glm::rotate(GetRotations().y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelTransform = GetScaleMat() * modelTransform;

	for (int i = 0; i < GetModel()->GetObjectsCount(); ++i) {
		objectGlobalTransform = GetModel()->GetObject_(i)->GetBasicObject()->globalTransform;

		transformMat = modelTransform * objectGlobalTransform;

		StaticObjectTransformedVert* transformedVert = new StaticObjectTransformedVert();
		objectTransformedVertices.push_back(transformedVert);

		for (int j = 0; j < GetModel()->GetObject_(i)->GetBasicObject()->controlPoints.size(); ++j) {
			vert = transformMat * GetModel()->GetObject_(i)->GetBasicObject()->controlPoints[j].coords;
			transformedVert->vertices.push_back(vert);
		}
	}
}
