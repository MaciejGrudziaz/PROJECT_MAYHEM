#pragma once
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <queue>
#include "WinAPIwindowManager.h"
#include "Camera.h"
#include "Model.h"
#include "Character.h"
#include "ImportFile.h"
//#include "ResourcesManager.h"
#include "Shader.h"
#include "wglext.h"

struct BasicShaderStruct {
	int shaderProgram;
	int projectionMatLoc;
	int viewMatLoc;
	bool render;

	BasicShaderStruct() :shaderProgram(-1), projectionMatLoc(-1), viewMatLoc(-1), render(true){}
};

class Graphics {
	static BasicShaderStruct ground;
	static BasicShaderStruct axis;

	static void DrawAxis();
	static void DrawGround();

	static void LoadBasicShaderStructs();

	static bool VSync(int on_off);	//0 - off, 1 - on
	static void InitShaders();

public:

	static void Init();
	static void Process();
	static void End();
};