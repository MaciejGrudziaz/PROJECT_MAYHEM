#pragma once
#include "Camera.h"
#include "Character.h"

class FollowCamera : public Camera {
	float distance;

	Character* followCharacter;

public:

	FollowCamera() :followCharacter(nullptr), distance(0.0f) {}

	FollowCamera(glm::vec3 position_, glm::vec3 lookVec_, glm::vec3 upVec_) :Camera(position_, lookVec_, upVec_), followCharacter(nullptr), distance(0.0f) {}

	void SetFollowCharacter(Character* character) {
		if (character != nullptr)
			followCharacter = character;
	}

	Character* GetFollowCharacter() { return followCharacter; }

	void SetDistance(float dist) { distance = dist; }
	float GetDistance()const { return distance; }

	void Update();

};
