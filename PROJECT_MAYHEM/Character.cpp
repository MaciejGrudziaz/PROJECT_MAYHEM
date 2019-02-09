#include "Character.h"

void Character::LoadShaderProgramToAll(Shader* shaderProgram) {
	if (shaderProgram != nullptr) {
		for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
			model.GetObject_(i)->LoadShader(shaderProgram);
		}
	}
}

void Character::LoadObjectShaderProgram(int index, Shader* shaderProgram) {
	if (shaderProgram != nullptr && index < model.GetObjectsCount() && index>=0) {
		model.GetObject_(index)->LoadShader(shaderProgram);
	}
}

void Character::LoadHitboxShaderProgramToAll(Shader* hitboxShaderProgram_) {
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		model.GetObject_(i)->LoadHitboxShader(hitboxShaderProgram_);
	}
}

void Character::LoadObjectHitboxShaderProgram(int index, Shader* shaderProgram) {
	if (shaderProgram != nullptr && index < model.GetObjectsCount() && index>=0) {
		model.GetObject_(index)->LoadHitboxShader(shaderProgram);
	}
}

void Character::LoadHitboxComputeShaderProgram(Shader* shaderProgram) {
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		model.GetObject_(i)->LoadHitboxComputeShader(shaderProgram);
	}
}

void Character::StartAnimation(int index, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(index,direction);
}

void Character::StartAnimation(int index, int startFrame, int stopFrame, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(index, startFrame, stopFrame,direction);
}

void Character::StartAnimation(int index, int startFrame, int stopFrame, int iterations, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(index,startFrame,stopFrame,iterations,direction);
}

void Character::StartAnimation(const char* name, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(name,direction);
}

void Character::StartAnimation(const char* name, int startFrame, int stopFrame, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(name, startFrame, stopFrame,direction);
}

void Character::StartAnimation(const char* name, int startFrame, int stopFrame, int iterations, int direction) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StartAnimation(name, startFrame, stopFrame, iterations,direction);
}

void Character::StopAnimations(int index) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StopAnimation(index);
}

void Character::StopAnimations(const char* name) {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StopAnimation(name);
}

void Character::StopAllAnimations() {
	for (int i = 0; i < model.GetObjectsCount(); ++i)
		model.GetObject_(i)->StopAllAnimations();
}

bool Character::IsAnimationActive(int index) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(index))
			return true;
	}
	return false;
}

bool Character::IsAnimationActive(const char* name) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(name))
			return true;
	}
	return false;
}

int Character::GetAnimationIterationsLeft(int index) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(index))
			return model.GetObject_(i)->GetAnimationIterationsLeft(index);
	}

	return -1;
}

int Character::GetAnimationIterationsLeft(const char* name) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(name))
			return model.GetObject_(i)->GetAnimationIterationsLeft(name);
	}

	return -1;
}

int Character::GetAnimationCurrentFrame(int index) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(index))
			return model.GetObject_(i)->GetAnimationCurrentFrame(index);
	}

	return -1;
}

int Character::GetAnimationCurrentFrame(const char* name) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(name))
			return model.GetObject_(i)->GetAnimationCurrentFrame(name);
	}

	return -1;
}

int Character::GetAnimationDirection(int index) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(index))
			return model.GetObject_(i)->GetAnimationDirection(index);
	}
	return -3;
}

int Character::GetAnimationDirection(const char* name) {
	for (int i = 0; i < model.GetObjectsCount(); ++i) {
		if (model.GetObject_(i)->IsAnimationActive(name))
			return model.GetObject_(i)->GetAnimationDirection(name);
	}
	return -3;
}

void Character::SetViewMatrix(glm::mat4 view_) {
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		model.GetObject_(i)->SetViewMatrix(view_);
	}
}

void Character::SetProjectionMatrix(glm::mat4 projection_) {
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		model.GetObject_(i)->SetProjectionMatrix(projection_);
	}
}

glm::vec3 Character::GetScaleVal() const {
	glm::vec3 scaleVal;
	scaleVal.x = scale[0][0];
	scaleVal.y = scale[1][1];
	scaleVal.z = scale[2][2];

	return scaleVal;
}

bool Character::IsObjectDynamic(int index) {
	if (index >= 0 && index < model.GetObjectsCount()) {
		DynamicObject* obj = dynamic_cast<DynamicObject*>(model.GetObject_(index));
		if (obj == nullptr) return false;
		else return true;
	}
}

void Character::Init() {
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		model.GetObject_(i)->Init();

		if (i == 0)
			size = model.GetObject_(i)->GetSize();
		else {
			for (int j = 0; j < 3; ++j) {
				if ((model.GetObject_(i)->GetSize())[j] > size[j])
					size[j] = (model.GetObject_(i)->GetSize())[j];
			}
		}
	}
}

void Character::Update() {
	//ProcessInputPackets();

	//if (transformUpdate) {
		transform = glm::translate(transform, position);
		transform = transform * glm::rotate(rotations.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = scale * transform;
	//}

	Object* obj = nullptr;
	glm::mat4 modelMat;
	for (unsigned i = 0; i < model.GetObjectsCount(); ++i) {
		obj = model.GetObject_(i);
		if (obj != nullptr)
			obj->SetModelMatrix(transform);
	}

	transform = glm::mat4(1.0);
	model.Update();
}

//---------------------------------------------------------------------------------------
//----------------------------------------PRIVATE----------------------------------------
//---------------------------------------------------------------------------------------

void Character::WriteErrorToFile(std::string message) {
	std::fstream file("LOGS/" + name + "_ERRORLOG.txt", std::ios::out | std::ios::app);
	assert(file.is_open());
	SYSTEMTIME time;
	GetSystemTime(&time);
	file << time.wDay << "." << time.wMonth << "." << time.wYear << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " - ";
	file << message << std::endl;
	file.close();
}

/*
void Character::ProcessInputPackets() {
	Packet* p;
	int move = 0x00;

	while (inPackets.PacketsAvailable()) {
		p = inPackets.GetPacket();

		if (p->class_ == 0) {
			if (p->group_ == 0) {
				if (p->id_ == 1) {
					move |= 0x01;
					position += (speed*CONSTVAL::targetFrameTime)*glm::vec3(lookVec.x, 0.0f, lookVec.z);
				}
				else if (p->id_ == 2) {
					move |= 0x02;
					position -= (speed*CONSTVAL::targetFrameTime)*glm::vec3(lookVec.x, 0.0f, lookVec.z);
				}
				else if (p->id_ == 3) {
					move |= 0x04;
					position += (speed*CONSTVAL::targetFrameTime)*(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(lookVec.x, 0.0f, lookVec.z)));
				}
				else if (p->id_ == 4) {
					move |= 0x08;
					position -= (speed*CONSTVAL::targetFrameTime)*(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(lookVec.x, 0.0f, lookVec.z)));
				}
			}
			if (p->group_ == 1) {
				int moveVal = static_cast<MouseMoveData*>(p->data)->moveVal;
				if (p->id_ == 1)
					rotations.x += (rotationSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 2)
					rotations.x -= (rotationSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 3)
					rotations.y += (rotationSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 4)
					rotations.y -= (rotationSpeed*CONSTVAL::targetFrameTime*moveVal);
			}
		}

		delete p;
	}

	glm::quat q, qx, qy;
	glm::mat3 rot;
	float sx = sin(rotations.x / 2.0f), sy = sin(-rotations.y / 2.0f);
	qx.w = cos(rotations.x / 2.0f);
	qx.x = sx * 1.0f;

	qy.w = cos(-rotations.y / 2.0f);
	qy.y = sy * 1.0f;

	q = glm::cross(qx, qy);
	rot = glm::mat3_cast(q);
	lookVec = glm::vec3(0.0f, 0.0f, 1.0f) * rot;

	glm::vec3 cameraPos = position;
	cameraPos.y += size.y;
	cameraPos -= (12.0f*lookVec);
	AutoList<Camera>::GetObj(0)->SetPosition(cameraPos);
	AutoList<Camera>::GetObj(0)->SetLookVec(lookVec);

	if (move == 0x00) {
		if (IsAnimationActive("WALK")) {
			StopAnimations("WALK");
			StartAnimation("WALK", 0, 20, 1, Animation::REVERSE);
		}
		if (IsAnimationActive("WALK_SIDE")) {
			StopAnimations("WALK_SIDE");
			StartAnimation("WALK_SIDE", 0, 20, 1, Animation::REVERSE);
		}
	}
	else {
		if ((move & 0x01) && (move & 0x02)) {
			StopAnimations("WALK");
			StartAnimation("WALK", 0, 20, 1, Animation::REVERSE);
		}
		else if (move & 0x01) {
			if (!IsAnimationActive("WALK") || GetAnimationDirection("WALK") == Animation::REVERSE)
				StartAnimation("WALK", 20, Animation::MAX_FRAME, Animation::NORMAL);
			if (IsAnimationActive("WALK_SIDE")) StopAnimations("WALK_SIDE");
		}
		else if (move & 0x02) {
			if (!IsAnimationActive("WALK") || GetAnimationDirection("WALK") == Animation::NORMAL)
				StartAnimation("WALK", 20, Animation::MAX_FRAME, Animation::REVERSE);
			if(IsAnimationActive("WALK_SIDE")) StopAnimations("WALK_SIDE");
		}
		else if ((move & 0x04) && (move & 0x08)){
			StopAnimations("WALK_SIDE");
			StartAnimation("WALK_SIDE", 0, 20, 1, Animation::REVERSE);
		}
		else if (move & 0x04) {
			if (!IsAnimationActive("WALK_SIDE") || GetAnimationDirection("WALK_SIDE") == Animation::NORMAL)
				StartAnimation("WALK_SIDE", 70, Animation::MAX_FRAME, Animation::REVERSE);
			if (IsAnimationActive("WALK")) StopAnimations("WALK");
		}
		else if (move & 0x08) {
			if (!IsAnimationActive("WALK_SIDE") || GetAnimationDirection("WALK_SIDE") == Animation::REVERSE)
				StartAnimation("WALK_SIDE", 70, Animation::MAX_FRAME, Animation::NORMAL);
			if (IsAnimationActive("WALK")) StopAnimations("WALK");
		}

		if (((move & 0x01) && (move & 0x08)) || ((move & 0x02) && (move & 0x04))) {
			if (!IsAnimationActive("RIGHT_WALK"))
				StartAnimation("RIGHT_WALK", 10, Animation::MAX_FRAME, Animation::NORMAL);
		}
		else {
			if (IsAnimationActive("RIGHT_WALK"))
				StopAnimations("RIGHT_WALK");
		}

		if (((move & 0x01) && (move & 0x04)) || ((move & 0x02) && (move & 0x08))) {
			if (!IsAnimationActive("LEFT_WALK"))
				StartAnimation("LEFT_WALK", 10, Animation::MAX_FRAME, Animation::NORMAL);
		}
		else {
			if (IsAnimationActive("LEFT_WALK"))
				StopAnimations("LEFT_WALK");
		}
	}
}
*/
