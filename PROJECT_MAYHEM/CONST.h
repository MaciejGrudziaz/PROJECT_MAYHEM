#pragma once
#include <math.h>

namespace CONSTVAL {
	const int targetFPS = 200;
	const float targetFrameTime = 1.0f / float(targetFPS);
	const float gAccelVal = 0.5f;
	const float fallAccelConst = sqrt(gAccelVal) / sqrt(2);
	const float deltaGroundVal = (gAccelVal*(targetFrameTime * float(targetFPS/1.5f))*(targetFrameTime * float(targetFPS/1.5f))) / 2.0f;
}
