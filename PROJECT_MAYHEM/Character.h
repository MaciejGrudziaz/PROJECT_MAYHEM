#pragma once
#include <vector>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include "Shader.h"
//#include "ResourcesManager.h"
#include "Model.h"
#include "AutoList.h"
#include "PacketManager.h"
#include "CONST.h"
//#include "Camera.h"

class Character:public AutoList<Character> {
	PacketManager inPackets;
	PacketManager outPackets;

	boost::mutex inPacketsMutex, inPacketsPriorityMutex;
	boost::mutex outPacketsMutex, outPacketsPriorityMutex;
	static const int maxPacketsCount = 100;

	std::string name;
	glm::vec3 position;
	glm::vec3 rotations;
	float speed;
	float rotationSpeed;
	glm::vec3 lookVec;
	glm::vec3 size;

	glm::mat4 scale;

	glm::mat4 transform;
	//glm::quat rotationQuat;
	bool transformUpdate;

	Model model;

	void WriteErrorToFile(std::string message);

	//void ProcessInputPackets();

protected:
	int GetInputPacketsCount_priority();
	bool InputPacketsAvailable_priority();
	Packet* PopInputPacket_priority();
	bool PushInputPacket_priority(Packet* packet);
	void GetAllInputPackets_priority(std::vector<Packet*>& packets);

	int GetOutputPacketsCount_priority();
	bool OutputPacketsAvailable_priority();
	Packet* PopOutputPacket_priority();
	bool PushOutputPacket_priority(Packet* packet);
	void GetAllOutputPackets_priority(std::vector<Packet*>& packets);

public:
	Character():position(glm::vec3(0.0f)),name("Character"),rotations(glm::vec3(0.0f)),transformUpdate(false), 
		speed(1.0f), rotationSpeed(1.0f), lookVec(glm::vec3(0.0f, 0.0f, 1.0f)), size(glm::vec3(0.0f)),scale(glm::mat4(1.0f)) { }
	Character(const char* name_):position(glm::vec3(0.0f)), name(name_), rotations(glm::vec3(0.0f)), transformUpdate(false), 
		speed(1.0f), rotationSpeed(1.0f), lookVec(glm::vec3(0.0f, 0.0f, 1.0f)), size(glm::vec3(0.0f)),scale(glm::mat4(1.0f)) { }

	void InitModel(BasicModel* basicModel_) { model.Init(basicModel_); }

	void LoadShaderProgramToAll(Shader* shaderProgram);
	void LoadObjectShaderProgram(int index, Shader* shaderProgram);

	void LoadHitboxShaderProgramToAll(Shader* hitboxShaderProgram_);
	void LoadObjectHitboxShaderProgram(int index, Shader* sahderProgram);

	void StartAnimation(int index, int direction);
	void StartAnimation(int index, int startFrame, int stopFrame, int direction);
	void StartAnimation(int index, int startFrame, int stopFrame, int iterations, int direction);

	void StartAnimation(const char* name, int direction);
	void StartAnimation(const char* name, int startFrame, int stopFrame, int direction);
	void StartAnimation(const char* name, int startFrame, int stopFrame, int iterations, int direction);

	void StopAnimations(int index);
	void StopAnimations(const char* name);
	void StopAllAnimations();

	bool IsAnimationActive(int index);
	bool IsAnimationActive(const char* name);

	int GetAnimationIterationsLeft(int index);
	int GetAnimationIterationsLeft(const char* name);

	int GetAnimationCurrentFrame(int index);
	int GetAnimationCurrentFrame(const char* name);

	int GetAnimationDirection(int index);
	int GetAnimationDirection(const char* name);

	void SetName(std::string name_) { name = name_; }
	void SetPosition(const glm::vec3& position_) { position = position_; transformUpdate = true; }
	void SetViewMatrix(glm::mat4 view_);
	void SetProjectionMatrix(glm::mat4 projection_);
	void Scale(glm::mat4 scaleMat) { scale = scaleMat; }
	void Scale(float x, float y, float z) { scale = glm::scale(glm::vec3(x, y, z)); }

	std::string GetName()const { return name; }
	glm::vec3 GetPosition()const { return position; }
	unsigned GetObjectsCount()const { return model.GetObjectsCount(); }
	float GetSpeed()const { return speed; }
	float GetRotationSpeed()const { return rotationSpeed; }
	glm::vec3 GetLookAtVec()const { return lookVec; }
	glm::vec3 GetSize()const { return size; }
	glm::vec3 GetRotations()const { return rotations; }
	glm::mat4 GetScaleMat()const { return scale; }
	glm::vec3 GetScaleVal()const;
	
	void SetSpeed(float speed_) { speed = speed_; }
	void SetRotationSpeed(float speed) { rotationSpeed = speed; }
	void SetLookAtVec(glm::vec3 lookAtVec) { lookVec = lookAtVec; }
	void SetRotation(glm::vec3 newRotation) { rotations = newRotation; }
	void UpdateRotation(glm::vec3 deltaRot) { rotations += deltaRot; }

	bool IsObjectDynamic(int index);

	int GetInputPacketsCount();
	bool InputPacketsAvailable();
	Packet* PopInputPacket();
	bool PushInputPacket(Packet* packet);
	void GetAllInputPackets(std::vector<Packet*>& packets);

	int GetOutputPacketsCount();
	bool OutputPacketsAvailable();
	Packet* PopOutputPacket();
	bool PushOutputPacket(Packet* packet);
	void GetAllOutputPackets(std::vector<Packet*>& packets);

	void Transform(const glm::mat4& transform_) { transform = transform_; transformUpdate = true; }

	virtual void Init();

	virtual void Update();

	virtual void Draw() { model.Draw(); }

	Model* GetModel() { return &model; }

	virtual ~Character() {
		while(inPackets.PacketsAvailable())
			delete inPackets.GetPacket();
	
		while (outPackets.PacketsAvailable())
			delete outPackets.GetPacket();
	}
};
