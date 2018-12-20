#include "Camera.h"

Camera::Camera():active(false) {
	position = glm::vec3(0.0f);
	lookVec = glm::vec3(0.0f, 0.0f, -1.0f);
	upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	lookAngle = 0.0f;
	speed = 2.0f;
	turnSensitivity = 10.0f;

	view = glm::lookAt(position, position + lookVec, upVec);
}

Camera::Camera(glm::vec3 position_, glm::vec3 lookVec_, glm::vec3 upVec_):position(position_),lookVec(lookVec_),upVec(upVec_),active(false) {
	lookAngle = 0.0f;
	view = glm::lookAt(position, position + lookVec, upVec);
	speed = 2.0f;
	turnSensitivity = 10.0f;
}

void Camera::Activate() {
	for (int i = 0; i < AutoList<Camera>::GetCount(); ++i) {
		if (AutoList<Camera>::GetObj(i)->active == true)
			AutoList<Camera>::GetObj(i)->active = false;
	}

	active = true;
}

void Camera::SetLookAngle(float lookAngle_) {
	lookAngle = lookAngle_;
	
	if (lookAngle < 0.0f) lookAngle = 360 + lookAngle;
	if (lookAngle > 360.0f) lookAngle = lookAngle - 360.0f;

	float x = -sin(glm::radians(lookAngle));
	float z = -cos(glm::radians(lookAngle));
	lookVec = glm::vec3(x, 0.0f, z);

	view = glm::lookAt(position, position + lookVec, upVec);
}
