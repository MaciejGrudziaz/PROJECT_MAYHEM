#include "PhysicsModule.h"

bool PhysicsModule::start = false;
bool PhysicsModule::run=true;
boost::thread PhysicsModule::physicsThread;

void PhysicsModule::Launch() {
	physicsThread = boost::thread(PhysicsModule());
}

void PhysicsModule::Process() {
	while (run) {
		if (start) {
			bool col;
			std::vector<glm::vec3> collisionNormals;

			for (int i = 0; i < AutoList<BasicCharacter>::GetCount() - 1; ++i) {
				for (int j = i+1; j < AutoList<BasicCharacter>::GetCount(); ++j) {
					col = CollisionDetection::CheckCollision(*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()));
					if (col == true) {
						CollisionDetection::GetCollisionNormals(*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()), collisionNormals);

						CollisionDetection::GetCollisionNormals(*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()), collisionNormals);

						//std::cout << "Collision! idx: "<<i<<", "<<j<<"\n";
						//for (int i = 0; i < collisionNormals.size(); ++i)
						//	std::cout << "n=[" << collisionNormals[i].x << ", " << collisionNormals[i].y << ", " << collisionNormals[i].z << "]\n";
					}
				}
			}
		}
	}
}

void PhysicsModule::End() {
	start = false;
	run = false;
	physicsThread.join();
}
