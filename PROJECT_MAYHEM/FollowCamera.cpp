#include "FollowCamera.h"

void FollowCamera::Update() {
	if (followCharacter != nullptr) {
		glm::vec3 pos, size, lookVec;

		pos = followCharacter->GetPosition();
		size = followCharacter->GetSize();
		lookVec = followCharacter->GetLookAtVec();

		pos.y += size.y;
		pos += (-lookVec)*12.0f;

		SetPosition(pos);
		SetLookVec(lookVec);
	}

	Camera::Update();
}
