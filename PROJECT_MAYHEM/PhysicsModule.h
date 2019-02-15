#pragma once
#include <boost/thread.hpp>
#include "CollisionDetection.h"
#include "Player.h"
#include "Map.h"
#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>

class PhysicsModule {
	enum PhysicsModuleType{Collision,Ground};

	static boost::thread collisionDetectionThread;
	static boost::thread groundPosThread;

	//static boost::mutex sync;
	//static boost::atomic<int> atomCnt2;

	PhysicsModuleType type;

	void ProcessCollision();

	void ProcessGroundPos();

	void GetGroundPos(BasicCharacter* character, Map* map);

public:

	PhysicsModule(PhysicsModuleType type_) :type(type_) {}

	static bool start;
	static bool run;

	static void LaunchCollisionDetection();
	static void LaunchGroundPos();

	void operator()() {
		switch (type) {
		case (PhysicsModuleType::Collision):
			ProcessCollision();
			break;
		case (PhysicsModuleType::Ground):
			ProcessGroundPos();
			break;
		}
	}

	static void End();

	~PhysicsModule() {}
};

//-----------------------------------------------------------------------------
//-----------------------------------PACKETS-----------------------------------
//-----------------------------------------------------------------------------


