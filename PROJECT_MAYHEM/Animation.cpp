#include "Animation.h"

AnimationManager::AnimationManager(BasicObject* basicObject_): basicObject(basicObject_),jointMatChange(false){
	baseAnimationFPS = 24;
	baseAnimationFrameTime = 1.0 / static_cast<float>(baseAnimationFPS);
	interpolationVal = 0.0;
	anyActive = 0.0;
	maxFrameCount = 0;
	jointsPrevMatrices = jointsNextMatrices = nullptr;

	glm::mat4 identity(1.0f);
	jointsMatricesCount = basicObject->skeleton.joints.size();
	jointsPrevMatrices = new float[16 * jointsMatricesCount];
	jointsNextMatrices = new float[16 * jointsMatricesCount];
	for (unsigned i = 0; i < jointsMatricesCount; ++i) {
		for (unsigned j = 0; j < 16; ++j) {
			jointsPrevMatrices[16 * i + j] = glm::value_ptr(identity)[j];
			jointsNextMatrices[16 * i + j] = glm::value_ptr(identity)[j];
		}
	}

	InitAnimationsVec();
}

void AnimationManager::Start(int animationIndex_, int direction_) {
	if (animationIndex_ < 0 || animationIndex_ >= animations.size()) {
		WriteErrorToFile("Wrong animation index: " + std::to_string(animationIndex_));
		throw std::exception("Wrong animation index!");
	}

	animations[animationIndex_]->active = true;
	if (direction_ == Animation::NORMAL) animations[animationIndex_]->currentFrame = 0;
	else if (direction_ == Animation::REVERSE) animations[animationIndex_]->currentFrame = animations[animationIndex_]->frameCount-1;
	animations[animationIndex_]->loop = true;
	animations[animationIndex_]->firstFrame = 0;
	animations[animationIndex_]->lastFrame = animations[animationIndex_]->frameCount - 1;
	animations[animationIndex_]->iterations = 0;
	animations[animationIndex_]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Start(int animationIndex_, int startFrame, int stopFrame, int direction_) {
	assert(animationIndex_ >= 0 && animationIndex_ < animations.size());

	if (stopFrame == Animation::MAX_FRAME) stopFrame = animations[animationIndex_]->frameCount - 1;
	if (stopFrame == Animation::CURR_FRAME) stopFrame = animations[animationIndex_]->currentFrame;
	if (startFrame == Animation::CURR_FRAME) startFrame = animations[animationIndex_]->currentFrame;

	assert(startFrame >= 0 && startFrame < animations[animationIndex_]->frameCount-1);
	assert(stopFrame >= 0 && stopFrame < animations[animationIndex_]->frameCount);

	animations[animationIndex_]->active = true;
	if (direction_ == Animation::NORMAL) animations[animationIndex_]->currentFrame = startFrame;
	else if (direction_ == Animation::REVERSE) animations[animationIndex_]->currentFrame = stopFrame;
	animations[animationIndex_]->loop = true;
	animations[animationIndex_]->firstFrame = startFrame;
	animations[animationIndex_]->lastFrame = stopFrame;
	animations[animationIndex_]->iterations = 0;
	animations[animationIndex_]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Start(int animationIndex_, int startFrame, int stopFrame, int iterations, int direction_) {
	assert(animationIndex_ >= 0 && animationIndex_ < animations.size());

	if (stopFrame == Animation::MAX_FRAME) stopFrame = animations[animationIndex_]->frameCount - 1;
	if (stopFrame == Animation::CURR_FRAME) stopFrame = animations[animationIndex_]->currentFrame;
	if (startFrame == Animation::CURR_FRAME) startFrame = animations[animationIndex_]->currentFrame;

	assert(startFrame >= 0 && startFrame < animations[animationIndex_]->frameCount - 1);
	assert(stopFrame >= 0 && stopFrame < animations[animationIndex_]->frameCount);
	assert(iterations > 0);

	animations[animationIndex_]->active = true;
	if (direction_ == Animation::NORMAL) animations[animationIndex_]->currentFrame = startFrame;
	else if (direction_ == Animation::REVERSE) animations[animationIndex_]->currentFrame = stopFrame;
	animations[animationIndex_]->loop = false;
	animations[animationIndex_]->firstFrame = startFrame;
	animations[animationIndex_]->lastFrame = stopFrame;
	animations[animationIndex_]->iterations = iterations;
	animations[animationIndex_]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Start(std::string animationName_, int direction_) {
	int index = -1;
	for (unsigned i = 0; i < animations.size(); ++i) {
		if (animations[i]->name == animationName_) {
			index = i;
			break;
		}
	}
	assert(index != -1);

	animations[index]->active = true;
	if (direction_ == Animation::NORMAL) animations[index]->currentFrame = 0;
	else if (direction_ == Animation::REVERSE) animations[index]->currentFrame = animations[index]->frameCount-1;
	animations[index]->loop = true;
	animations[index]->firstFrame = 0;
	animations[index]->lastFrame = animations[index]->frameCount-1;
	animations[index]->iterations = 0;
	animations[index]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Start(std::string animationName_, int startFrame, int stopFrame, int direction_) {
	int index = -1;
	for (unsigned i = 0; i < animations.size(); ++i) {
		if (animations[i]->name == animationName_) {
			index = i;
			break;
		}
	}
	assert(index != -1);

	if (stopFrame == Animation::MAX_FRAME) stopFrame = animations[index]->frameCount - 1;
	if (stopFrame == Animation::CURR_FRAME) stopFrame = animations[index]->currentFrame;
	if (startFrame == Animation::CURR_FRAME) startFrame = animations[index]->currentFrame;

	assert(startFrame >= 0 && startFrame < animations[index]->frameCount - 1);
	assert(stopFrame >= 0 && stopFrame < animations[index]->frameCount);

	animations[index]->active = true;
	if(direction_ == Animation::NORMAL) animations[index]->currentFrame = startFrame;
	else if(direction_ == Animation::REVERSE) animations[index]->currentFrame = stopFrame;
	animations[index]->loop = true;
	animations[index]->firstFrame = startFrame;
	animations[index]->lastFrame = stopFrame;
	animations[index]->iterations = 0;
	animations[index]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Start(std::string animationName_, int startFrame, int stopFrame, int iterations, int direction_) {
	int index = -1;
	for (unsigned i = 0; i < animations.size(); ++i) {
		if (animations[i]->name == animationName_) {
			index = i;
			break;
		}
	}
	assert(index != -1);

	if (stopFrame == Animation::MAX_FRAME) stopFrame = animations[index]->frameCount - 1;
	if (stopFrame == Animation::CURR_FRAME) stopFrame = animations[index]->currentFrame;
	if (startFrame == Animation::CURR_FRAME) startFrame = animations[index]->currentFrame;

	assert(startFrame >= 0 && startFrame < animations[index]->frameCount - 1);
	assert(stopFrame >= 0 && stopFrame < animations[index]->frameCount);
	assert(iterations > 0);

	animations[index]->active = true;
	if (direction_ == Animation::NORMAL) animations[index]->currentFrame = startFrame;
	else if (direction_ == Animation::REVERSE) animations[index]->currentFrame = stopFrame;
	animations[index]->loop = false;
	animations[index]->firstFrame = startFrame;
	animations[index]->lastFrame = stopFrame;
	animations[index]->iterations = iterations;
	animations[index]->direction = direction_;
	anyActive = true;
	prevTime = boost::chrono::high_resolution_clock::now();
}

void AnimationManager::Stop(int animationIndex_) {
	if (animationIndex_ < 0 || animationIndex_ >= animations.size()) {
		WriteErrorToFile("Wrong animation index: " + std::to_string(animationIndex_));
		throw std::exception("Wrong animation index!");
	}

	animations[animationIndex_]->active = false;
	//animations[animationIndex_]->currentFrame = 0;
	animations[animationIndex_]->iterations = 0;
	animations[animationIndex_]->loop = false;
	anyActive = false;
	for (Animation* anim : animations) {
		if (anim->active == true) {
			anyActive = true;
			break;
		}
	}
}

void AnimationManager::Stop(const char* name) {
	int index = -1;

	for (int i = 0; i < animations.size(); ++i) {
		if (animations[i]->name == name) {
			index = i;
			break;
		}
	}

	assert(index != -1);

	animations[index]->active = false;
	//animations[index]->currentFrame = 0;
	animations[index]->iterations = 0;
	animations[index]->loop = false;
	anyActive = false;
	for (Animation* anim : animations) {
		if (anim->active == true) {
			anyActive = true;
			break;
		}
	}
}

void AnimationManager::StopAll() {
	for (int i = 0; i < animations.size(); ++i) {
		animations[i]->active = false;
		//animations[i]->currentFrame = 0;
		animations[i]->iterations = 0;
		animations[i]->loop = false;
	}

	anyActive = false;
}

bool AnimationManager::IsActive(int animationIndex) {
	if (animationIndex >= 0 && animationIndex < animations.size())
		return animations[animationIndex]->active;
	else return false;
}

bool AnimationManager::IsActive(const char* name) {
	for (Animation* anim : animations) {
		if (anim->name == name)
			return anim->active;
	}

	return false;
}

int AnimationManager::GetIterationsLeft(int animationIndex) {
	if (animationIndex >= 0 && animationIndex < animations.size())
		return animations[animationIndex]->iterations;
	else return -1;
}

int AnimationManager::GetIterationsLeft(const char* name) {
	for (Animation* anim : animations) {
		if (anim->name == name)
			return anim->iterations;
	}

	return -1;
}

int AnimationManager::GetCurrentFrame(int animationIndex) {
	if (animationIndex >= 0 && animationIndex < animations.size())
		return animations[animationIndex]->currentFrame;
	else return -1;
}

int AnimationManager::GetCurrentFrame(const char* name) {
	for (Animation* anim : animations) {
		if (anim->name == name)
			return anim->currentFrame;
	}

	return -1;
}

int AnimationManager::GetDirection(int animationIndex) {
	if (animationIndex >= 0 && animationIndex < animations.size()) {
		return animations[animationIndex]->direction;
	}
	return -1;
}

int AnimationManager::GetDirection(const char* name) {
	for (Animation* anim : animations) {
		if (anim->name == name)
			return anim->direction;
	}
	return -1;
}

void AnimationManager::Process() {
	if (anyActive) {
		if (interpolationVal >= 1.0) {
			++currentFrameCount;
			jointMatChange = true;
			if (currentFrameCount > maxFrameCount - 1) {
				currentFrameCount = 0;
				time = 0.0;
			}
			//time = 0.0;
			interpolationVal = 0.0;

			SetAnimationsNextFrame();

			for (unsigned i = 0; i < jointsMatricesCount; ++i) {
				if (basicObject->skeleton.joints[i]->animations.size() > 0) {
					glm::mat4 nextJointTransform,currJointTransform;
					//currJointTransform = GetAnimationsCurrJointMatrix(i);
					nextJointTransform = GetAnimationsNextJointMatrix(i);

					//if (currJointTransform != nextJointTransform) {
						//void* prevMat, *nextMat;
						//prevMat = glMapNamedBufferRange(shaderManager->GetJointsPrevMatricesBuffer(), i*(16 * sizeof(float)), 16 * sizeof(float), GL_MAP_WRITE_BIT);
						//nextMat = glMapNamedBufferRange(shaderManager->GetJointsNextMatricesBuffer(), i*(16 * sizeof(float)), 16 * sizeof(float), GL_MAP_WRITE_BIT);
						//memcpy(prevMat, glm::value_ptr(currJointTransform), 16 * sizeof(float));
						//memcpy(nextMat, glm::value_ptr(nextJointTransform), 16 * sizeof(float));
						//glUnmapNamedBuffer(shaderManager->GetJointsPrevMatricesBuffer());
						//glUnmapNamedBuffer(shaderManager->GetJointsNextMatricesBuffer());
					//}

					for (unsigned j = 0; j < 4; ++j) {
						for (unsigned k = 0; k < 4; ++k) {
							jointsPrevMatrices[16 * i + j * 4 + k] = jointsNextMatrices[16 * i + j * 4 + k];
							jointsNextMatrices[16 * i + j * 4 + k] = nextJointTransform[j][k];
						}
					}
				}
			}

			//void *prevMat, *nextMat;
			//prevMat= glMapNamedBufferRange(shaderManager->GetJointsPrevMatricesBuffer(), 0, jointsMatricesCount * 16 * sizeof(float), GL_MAP_WRITE_BIT);
			//nextMat= glMapNamedBufferRange(shaderManager->GetJointsNextMatricesBuffer(), 0, jointsMatricesCount * 16 * sizeof(float), GL_MAP_WRITE_BIT);
			//memcpy(prevMat, jointsPrevMatrices, jointsMatricesCount * 16 * sizeof(float));
			//memcpy(nextMat, jointsNextMatrices, jointsMatricesCount * 16 * sizeof(float));
			//glUnmapNamedBuffer(shaderManager->GetJointsPrevMatricesBuffer());
			//glUnmapNamedBuffer(shaderManager->GetJointsNextMatricesBuffer());
		}
		else {
			jointMatChange = false;
			time += CONSTVAL::targetFrameTime;
			prevTime = boost::chrono::high_resolution_clock::now();
			interpolationVal = (time / baseAnimationFrameTime) - static_cast<float>(currentFrameCount);
		}
	}
}

/*glm::mat4 AnimationManager::GetJointPrevFrameTransformMatrix(int jointIdx) {
	if (jointIdx >= 0 && jointIdx < basicObject->skeleton.joints.size()) {
		glm::mat4 prevFrame = glm::mat4(1.0f);

		for (unsigned i = 0; i < animations.size(); ++i) 
			prevFrame *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->currentFrame]->globalTransform*basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);

		return prevFrame;
	}
	else throw std::exception();
}

glm::mat4 AnimationManager::GetJointNextFrameTransformMatrix(int jointIdx) {
	if (jointIdx >= 0 && jointIdx < basicObject->skeleton.joints.size()) {
		glm::mat4 nextFrame = glm::mat4(1.0f);;

		for (unsigned i = 0; i < animations.size(); ++i) {
				if (animations[i]->currentFrame<animations[i]->frameCount - 1)
					nextFrame *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->currentFrame]->globalTransform*basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
				else nextFrame *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[0]->globalTransform*basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
		}

		return nextFrame;
	}
	else throw std::exception();
}

glm::vec4 AnimationManager::TransformVertexByCurrentJointMatrix(glm::vec4 vertex, int jointIdx) {
	if (jointIdx >= 0 && jointIdx < basicObject->skeleton.joints.size()) {
		glm::mat4 prevFrame, nextFrame;
		glm::vec4 outVertex;
		prevFrame = GetJointPrevFrameTransformMatrix(jointIdx);
		nextFrame = GetJointNextFrameTransformMatrix(jointIdx);

		outVertex = ((1.0f - interpolationVal)*prevFrame + interpolationVal * nextFrame)*vertex;

		return outVertex;
	}
	else throw std::exception();
}*/

//---------------------------------------------------------------------------------------
//----------------------------------------PRIVATE----------------------------------------
//---------------------------------------------------------------------------------------

void AnimationManager::InitAnimationsVec() {
	maxFrameCount = 0;
	if (basicObject->animationsInfo.size() > 0)
		maxFrameCount = basicObject->animationsInfo[0].frameCount;
	for (unsigned i = 0; i < basicObject->animationsInfo.size(); ++i) {
		animations.push_back(new Animation(basicObject->animationsInfo[i].name, basicObject->animationsInfo[i].frameCount));
		if (basicObject->animationsInfo[i].frameCount > maxFrameCount)
			maxFrameCount = basicObject->animationsInfo[i].frameCount;
	}
}

void AnimationManager::SetAnimationsNextFrame() {
	for (Animation* animation : animations) {
		if (animation->active) {
			//animation->currentFrame=animation->currentFrame+1+animation->skipFrame;
			//++animation->currentFrame;
			if (animation->direction == Animation::NORMAL) {
				animation->currentFrame += animation->speed;
				if (animation->currentFrame > animation->lastFrame - 1) {
					if (animation->loop)
						animation->currentFrame = animation->firstFrame + (animation->currentFrame - animation->lastFrame);
					else {
						if (animation->iterations > 1) {
							--animation->iterations;
							animation->currentFrame = animation->firstFrame + (animation->currentFrame - animation->lastFrame);
						}
						else {
							animation->iterations = 0;
							animation->currentFrame = 0;
							animation->active = false;
						}
					}
				}
			}
			else if(animation->direction == Animation::REVERSE){
				animation->currentFrame -= animation->speed;
				if (animation->currentFrame < animation->firstFrame + 1) {
					if (animation->loop)
						animation->currentFrame = animation->lastFrame - (animation->firstFrame - animation->currentFrame);
					else {
						if (animation->iterations > 1) {
							--animation->iterations;
							animation->currentFrame = animation->lastFrame - (animation->firstFrame - animation->currentFrame);
						}
						else {
							animation->currentFrame = 0;
							animation->active = false;
						}
					}
				}
			}
		}
	}
}

glm::mat4 AnimationManager::GetAnimationsCurrJointMatrix(unsigned jointIdx) {
	glm::mat4 jointTransformMatrix;
	if (basicObject->skeleton.joints[jointIdx]->animations.size() > 0) {
		for (unsigned i = 0; i < animations.size(); ++i) {
			if (animations[i]->active) {
				if (animations[i]->currentFrame < animations[i]->frameCount)
					jointTransformMatrix = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->currentFrame]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
				else jointTransformMatrix = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[0]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
			}
		}
	}

	return jointTransformMatrix;
}

glm::mat4 AnimationManager::GetAnimationsNextJointMatrix(unsigned jointIdx) {
	glm::mat4 jointTransformMatrix(1.0f);
	if (basicObject->skeleton.joints[jointIdx]->animations.size() > 0) {
		for (unsigned i = 0; i < animations.size(); ++i) {
			if (animations[i]->active) {
				int currFrameIdx = static_cast<int>(animations[i]->currentFrame);
				float residue = animations[i]->currentFrame - static_cast<float>(currFrameIdx);

				if ((animations[i]->speed-static_cast<float>(static_cast<int>(animations[i]->speed))==0.0f)) {
					if (animations[i]->direction == Animation::NORMAL) {
						if (currFrameIdx < animations[i]->lastFrame)
							jointTransformMatrix *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx + 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
						else jointTransformMatrix *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->firstFrame]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
					}
					else {
						if (currFrameIdx > animations[i]->firstFrame)
							jointTransformMatrix *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx - 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
						else jointTransformMatrix *= (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->lastFrame]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);// *jointTransformMatrix;
					}
				}
				else {
					glm::mat4 prevMat, nextMat,outMat;
					glm::vec4 outTrans;
					glm::quat prevQuat, nextQuat;

					if (animations[i]->direction == Animation::NORMAL) {
						if (currFrameIdx < animations[i]->lastFrame) {
							prevMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
							nextMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx + 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
						}
						else {
							prevMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->firstFrame]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
							nextMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->firstFrame + 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
						}
					}
					else {
						if (currFrameIdx > animations[i]->firstFrame) {
							prevMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
							nextMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[currFrameIdx - 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
						}
						else {
							prevMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->lastFrame]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
							nextMat = (basicObject->skeleton.joints[jointIdx]->animations[i]->frames[animations[i]->lastFrame - 1]->globalTransform * basicObject->skeleton.joints[jointIdx]->globalBindposeInverse);
						}
					}

					outTrans = prevMat[3] + residue * (nextMat[3] - prevMat[3]);

					prevQuat = glm::quat_cast(prevMat);
					nextQuat = glm::quat_cast(nextMat);

					outMat = glm::mat4_cast(glm::mix(prevQuat, nextQuat, residue));
					outMat[3] = outTrans;

					jointTransformMatrix *= outMat;
				}
			}
		}
	}

	return jointTransformMatrix;
}

void AnimationManager::WriteErrorToFile(std::string message) {
	std::fstream file("LOGS/MODEL_ANIMATION_ERRORLOG.txt", std::ios::out | std::ios::app);
	assert(file.is_open());
	SYSTEMTIME time;
	GetSystemTime(&time);
	file << time.wDay << "." << time.wMonth << "." << time.wYear << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " - ";
	file << message << std::endl;
	file.close();
}
