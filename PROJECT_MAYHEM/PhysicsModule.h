#pragma once
#include <boost/thread.hpp>
#include "CollisionDetection.h"
#include "Player.h"
#include "Map.h"
#include <boost/chrono.hpp>

class PhysicsModule {
	static boost::thread physicsThread;
	static const int freq;
	static const int simTime; //us

public:
	static bool start;
	static bool run;

	static void Launch();

	void Process();

	void operator()() { Process(); }

	static void End();

	~PhysicsModule() {}
};

//-----------------------------------------------------------------------------
//-----------------------------------PACKETS-----------------------------------
//-----------------------------------------------------------------------------


