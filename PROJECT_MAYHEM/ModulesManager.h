#pragma once
#include <boost/chrono/chrono.hpp>
#include "Graphics.h"
#include "InputModule.h"
#include "ResourcesManager.h"
#include "CONST.h"
#include "CollisionDetection.h"
#include <iostream>

class ModulesManager {
	//static const int targetFPS;
	//static const float targetFrameTime;
	static float timeResidue;
	static float lastFrameTime;
	static boost::chrono::high_resolution_clock::time_point prevTime;
	static std::queue<float> frameTime;
	static int logFlag;

	static int GetFrameCount();

	static void StartFrameTimeMeasurment() { prevTime = boost::chrono::high_resolution_clock::now(); }
	static void StopFrameTimeMeasurment();

	static void AddFrameTimeToQueue(float time);

	static void WriteFPSToFile();

public:

	static void Init();

	static void Process();

	static void End();

};