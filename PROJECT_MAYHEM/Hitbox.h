#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <vector>
#include <string>

struct Hitbox {
	struct Axis {
		glm::vec3 x;
		glm::vec3 y;
		glm::vec3 z;

		Axis() :x(glm::vec3(1.0f, 0.0f, 0.0f)), y(glm::vec3(0.0f, 1.0f, 0.0f)), z(glm::vec3(0.0f, 0.0f, 1.0f)) {}
	};
	struct MoveValues {
		float xMove[2];
		float yMove[2];
		float zMove[2];

		MoveValues() :xMove{ 0.0f,0.0f }, yMove{ 0.0f,0.0f }, zMove{ 0.0f,0.0f } {}
	};
	struct IndexGroup {
		std::set<int> idx;
		int baseIdx;
		int status;
		enum Status { OPEN, CLOSED, IN_USE };

		IndexGroup(int baseIdx_) :baseIdx(baseIdx_), status(OPEN), idx(std::set<int>()) {}
	};

	Hitbox(const Hitbox& hitbox) {
		name = hitbox.name;
		for (int i = 0; i < hitbox.controlPointsIdx.size(); ++i)
			controlPointsIdx.push_back(hitbox.controlPointsIdx[i]);
		localAxis = hitbox.localAxis;
		jointIdx = hitbox.jointIdx;
		damageMultiplier = hitbox.damageMultiplier;

		memcpy(basicVertices, hitbox.basicVertices, 8 * sizeof(glm::vec4));
	}

	std::string name;
	std::vector<int> controlPointsIdx;

	Axis localAxis;
	//glm::vec4 initVertices[8];
	glm::vec3 basicVertices[8];
	glm::vec3 transformVertices[8];
	int jointIdx;
	static unsigned indices[12 * 2];

	double damageMultiplier;

	Hitbox();
};
