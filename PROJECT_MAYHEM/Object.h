#pragma once
#include <GL/glew.h>
#include <Windows.h>
#include <boost/chrono.hpp>
#include <assert.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <map>
#include <set>
#include "BasicObject.h"
//#include "ResourcesManager.h"
#include "Shader.h"
#include "ObjectShaderManager.h"
#include "Animation.h"
#include "Hitbox.h"

const std::string g_modelsDirectory = "MODELS/";

class Object {
protected:
	AnimationManager* animationManager;
	BasicObject* basicObject;
	ObjectShaderManager* shaderManager;

	int objectBufferVertexAttribCount;
	int hitboxObjectBufferVertexAttribCount;

	float* vertices;
	unsigned verticesCount;
	float* hitboxVertices;
	float* hitboxOutVertices;
	unsigned hitboxVerticesCount;	//��czna liczba zmiennych - liczba wierzcholk�w * 4(parametry na wierzcho�ek)
	int updateHitboxVerticesInBuffer;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 size;
	glm::vec3 minVec;
	glm::vec3 maxVec;

	int textureAvailable;

	typedef std::map<int, Hitbox*> HitboxMap;
	HitboxMap hitboxes;
	Hitbox* mainHitbox;
	bool showAllHitboxes;
	bool showHitbox;
	int showHitboxIdx;

	void WriteErrorToFile(std::string message);

	void GetSizeFromMainHitbox();

	virtual void UpdateHitboxes() = 0;
	virtual void SetVerticesBuffer()=0;

public:
	Object();
	
	BasicObject* GetBasicObject() { return basicObject; }

	void LoadHitboxBufferData();
	void AddHitbox(Hitbox* hitbox) { hitboxes.insert(HitboxMap::value_type(hitbox->jointIdx, hitbox)); }
	void AddMainHitbox(Hitbox* hitbox) { mainHitbox = hitbox; GetSizeFromMainHitbox(); }
	Hitbox* GetHitbox(int idx) {
		HitboxMap::iterator it = (hitboxes.find(idx));
		if (it != hitboxes.end()) return it->second;
		else return nullptr;
	}
	Hitbox* GetMainHitbox() { return mainHitbox; }
	int GetHitboxCount()const { return hitboxes.size(); }

	void LoadHitboxDataFromShaders();

	void LoadShader(Shader* shaderProgram_);

	void LoadHitboxShader(Shader* hitboxBasicShaderProgram_);
	void LoadHitboxComputeShader(Shader* hitboxComputeShaderProgram);

	int LoadTexture(std::string filename);

	int StartAnimation(int animationIndex_, int direction);
	int StartAnimation(int animationIndex_,int startFrame,int stopFrame, int direction);
	int StartAnimation(int animationIndex_,int startFrame,int stopFrame,int iterations, int direction);

	int StartAnimation(const char* name, int direction);
	int StartAnimation(const char* name, int startFrame, int stopFrame, int direction);
	int StartAnimation(const char* name, int startFrame, int stopFrame, int iterations, int direction);

	int StopAnimation(int animationIndex_);
	int StopAnimation(const char* name);
	int StopAllAnimations();

	bool IsAnimationActive(int animationIndex);
	bool IsAnimationActive(const char* name);

	int GetAnimationIterationsLeft(int animationIndex);
	int GetAnimationIterationsLeft(const char* name);

	int GetAnimationCurrentFrame(int animationIndex);
	int GetAnimationCurrentFrame(const char* name);

	int GetAnimationDirection(int animationIndex);
	int GetAnimationDirection(const char* name);

	int GetAnimationsCount()const { if (animationManager != nullptr) return animationManager->GetAnimationsCount(); else return 0; }
	Animation* GetAnimation(int index);
	Animation* GetAnimation(const char* name);
	int Animation_GetSpeed(int index);
	int Animation_GetSpeed(const char* name);
	int Animation_SetSpeed(int index, float speed);
	int Animation_SetSpeed(const char* name, float speed);
	const char* Animation_GetName(int index);
	int Animation_SetName(int index, const char* name);

	void SetBasicObject(BasicObject* basicObject_) { basicObject = basicObject_; /*LoadHitboxes();*/ }

	glm::mat4 GetModelMatrix()const { return model; }
	glm::mat4 GetViewMatrix()const { return view; }
	glm::mat4 GetProjectionMatrix()const { return projection; }

	glm::vec3 GetSize()const { return size; }
	glm::vec3 GetMinVec()const { return minVec; }
	glm::vec3 GetMaxVec()const { return maxVec; }

	ObjectShaderManager* GetShaderManager() { return shaderManager; }
	AnimationManager* GetAnimationManager() { return animationManager; }

	const float* GetHitboxOutVertices()const { return hitboxOutVertices; }
	const float* GetHitboxInVertices()const { return hitboxVertices; }
	unsigned GetHitboxVerticesBufferSize()const { return hitboxVerticesCount; }

	void SetModelMatrix(glm::mat4 model_) { model = model_; }
	void SetViewMatrix(glm::mat4 view_) { view = view_; }
	void SetProjectionMatrix(glm::mat4 projection_) { projection = projection_; }

	void ShowAllHitboxes() { showAllHitboxes = true; showHitbox = false; }
	bool ShowHitbox(int idx);
	void ShowMainHitbox() { showHitbox = true; showAllHitboxes = false; showHitboxIdx = -1; }
	void HideHitboxes() { showAllHitboxes = false; showHitbox = false; }

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~Object() {
		delete[]vertices;
		delete[]hitboxVertices;
		delete[]hitboxOutVertices;
		delete shaderManager;
		delete animationManager;
		delete mainHitbox;

		for (HitboxMap::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it)
			delete it->second;
		hitboxes.clear();
	}


};

class DynamicObject :public Object {
	void SetVerticesBuffer();

	void UpdateHitboxes();

public:
	DynamicObject() { shaderManager = new DynamicObjectShaderManager(); }

	void Init();

	void Update();

	void Draw();
};

class StaticObject :public Object {
	void SetVerticesBuffer();

	void UpdateHitboxes();

public:
	StaticObject() { shaderManager = new StaticObjectShaderManager(); }

	void Init();

	void Update();

	void Draw();
};