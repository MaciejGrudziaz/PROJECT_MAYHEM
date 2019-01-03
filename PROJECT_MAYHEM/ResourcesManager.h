#pragma once
#include "ImportFile.h"
#include "WinAPIwindowManager.h"
//#include "Character.h"
//#include "BasicCharacter.h"
#include "Player.h"
#include "Map.h"
//#include "Camera.h"
#include "FollowCamera.h"

class ResourcesManager {
	ResourcesManager() {}
	ResourcesManager(const ResourcesManager&) {}
	ResourcesManager& operator=(const ResourcesManager&) {}

	template <class T>
	static T* LoadModel(const char* filename, const char* objectName);

	static void DeleteModel(int idx);

public:

	static void Init();

	static void Clear();
};
