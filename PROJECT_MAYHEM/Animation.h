#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <boost/chrono.hpp>
#include "Shader.h"
#include "BasicObject.h"
#include "CONST.h"
//#include "ResourcesManager.h"

//#define MAX_FRAME 0xFFFFFFFF

struct Animation {
	static const int CURR_FRAME = 0xFFFFFFFE;
	static const int MAX_FRAME = 0xFFFFFFFF;
	static const int NORMAL = 0x00;
	static const int REVERSE = 0x01;

	std::string name;
	bool active;
	float currentFrame;
	int frameCount;

	//bool switchToFirst;

	int firstFrame;
	int lastFrame;
	float speed;

	int iterations;
	bool loop;

	char direction;

	Animation(std::string name_, int frameCount_) :name(name_), frameCount(frameCount_), active(false), currentFrame(0.0f), 
		firstFrame(0), lastFrame(frameCount - 1), speed(1.0f), loop(false),iterations(0),direction(0x00){}
};

class AnimationManager {
	int baseAnimationFPS;
	float baseAnimationFrameTime;

	BasicObject* basicObject;

	bool jointMatChange;
	float* jointsPrevMatrices;
	float* jointsNextMatrices;
	float* identityMatricesJointsBuffer;
	unsigned jointsMatricesCount;

	float interpolationVal;

	int maxFrameCount;
	int currentFrameCount;
	bool anyActive;

	boost::chrono::high_resolution_clock::time_point prevTime;
	float time;

	std::vector<Animation*> animations;

	void InitAnimationsVec();

	void SetAnimationsNextFrame();

	glm::mat4 GetAnimationsNextJointMatrix(unsigned jointIdx);
	glm::mat4 GetAnimationsCurrJointMatrix(unsigned jointIdx);

	void WriteErrorToFile(std::string message);

public:
	AnimationManager(BasicObject* object_);

	void Start(int animationIndex_, int direction_);
	void Start(int animationIndex_, int startFrame, int stopFrame, int direction_);
	void Start(int animationIndex_, int startFrame, int stopFrame, int iterations, int direction_);
	void Start(std::string animationName_, int direction_);
	void Start(std::string animationName_, int startFrame, int stopFrame, int direction_);
	void Start(std::string animationName_, int startFrame, int stopFrame, int iterations, int direction_);
	void Stop(int animationIndex_);
	void Stop(const char* name);
	void StopAll();

	bool IsActive(int animationIndex);
	bool IsActive(const char* name);

	int GetIterationsLeft(int animationIndex);
	int GetIterationsLeft(const char* name);

	int GetCurrentFrame(int animationIndex);
	int GetCurrentFrame(const char* name);

	int GetDirection(int animationIndex);
	int GetDirection(const char* name);

	int GetAnimationsCount()const { return animations.size(); }

	void Process();

	unsigned GetJointsMatricesCount()const { return jointsMatricesCount; }
	float GetInterpolationVal()const { return interpolationVal; }
	const float* GetJointsNextTransformMatrices()const { return jointsNextMatrices; }
	const float* GetJointsPreviousTransformMatrices()const { return jointsPrevMatrices; }
	bool GetJointsMatChangeStatus()const { return jointMatChange; }

	//glm::mat4 GetJointPrevFrameTransformMatrix(int jointIdx);
	//glm::mat4 GetJointNextFrameTransformMatrix(int jointIdx);
	//glm::vec4 TransformVertexByCurrentJointMatrix(glm::vec4 vertex, int jointIdx);

	Animation* GetAnimation(int index) { if (index >= 0 && index <= animations.size() - 1) return animations[index]; else return nullptr; }
	int GetBaseAnimtionFPS()const { return baseAnimationFPS; }

	~AnimationManager() {
		delete[]jointsPrevMatrices;
		delete[]jointsNextMatrices;
		delete[]identityMatricesJointsBuffer;
	}

};