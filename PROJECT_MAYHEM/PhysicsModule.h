#pragma once
#include <boost/thread.hpp>
#include "CollisionDetection.h"
#include "Player.h"
#include "Map.h"

class PhysicsModule {
	static boost::thread physicsThread;

public:
	static bool start;
	static bool run;

	static void Launch();

	void Process();

	void operator()() { Process(); }

	static void End();

	~PhysicsModule() {}
};