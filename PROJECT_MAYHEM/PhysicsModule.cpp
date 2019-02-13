#include "PhysicsModule.h"

bool PhysicsModule::start = false;
bool PhysicsModule::run=true;
boost::thread PhysicsModule::physicsThread;
const int PhysicsModule::freq = 120;
const int PhysicsModule::simTime = int((1.0f / (float)PhysicsModule::freq)*1000000);

void PhysicsModule::Launch() {
	physicsThread = boost::thread(PhysicsModule());
}

void PhysicsModule::Process() {
	bool col;
	std::vector<glm::vec3> collisionNormals;
	boost::chrono::high_resolution_clock::time_point prevTime;
	int timePassed;

	while (run) {
		if (start) {
			prevTime = boost::chrono::high_resolution_clock::now();

			collisionNormals.clear();
			for (int i = 0; i < AutoList<BasicCharacter>::GetCount() - 1; ++i) {
				for (int j = i+1; j < AutoList<BasicCharacter>::GetCount(); ++j) {
					col = CollisionDetection::CheckCollision(*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()));
					if (col == true) {
						CollisionData* colData = new CollisionData();
						Packet* packet = new Packet(1, 0, 0, colData);

						CollisionDetection::GetCollisionNormals(*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()), colData->normals);

						AutoList<BasicCharacter>::GetObj(i)->PushInputPacket(packet);

						colData = new CollisionData();
						packet = new Packet(1, 0, 1, colData);

						CollisionDetection::GetCollisionNormals(*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()), colData->normals);

						AutoList<BasicCharacter>::GetObj(j)->PushInputPacket(packet);

						//std::cout << "Collision! idx: "<<i<<", "<<j<<"\n";
						//for (int i = 0; i < collisionNormals.size(); ++i)
						//	std::cout << "n=[" << collisionNormals[i].x << ", " << collisionNormals[i].y << ", " << collisionNormals[i].z << "]\n";
					}
				}
			}

			for (int i = 0; i < AutoList<BasicCharacter>::GetCount(); ++i) {
				if (AutoList<Map>::GetCount() > 0) {
					BasicCharacter* character = AutoList<BasicCharacter>::GetObj(i);
					Map* map = AutoList<Map>::GetObj(0);
					for (int j = 0; j < map->GetModel()->GetObjectsCount(); ++j) {
						col = CollisionDetection::CheckCollision(*(character->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(map->GetModel()->GetObject_(j)->GetMainHitbox()));

						if (col == true) {
							if (map->GetModel()->GetObject_(j)->GetBasicObject()->name != map->GetGroundObject(0)->GetBasicObject()->name) {
								CollisionData* colData = new CollisionData();
								Packet* packet = new Packet(1, 0, 0, colData);

								CollisionDetection::GetCollisionNormals(*(character->GetModel()->GetObject_(0)->GetMainHitbox()),
									*(map->GetModel()->GetObject_(j)->GetMainHitbox()), colData->normals);

								character->PushInputPacket(packet);
							}
							else {
								GroundCollision* colData = new GroundCollision(0, j);
								Packet* packet = new Packet(1, 0, 1, colData);

								character->PushInputPacket(packet);
							}

							//std::cout << "Collision: map obj name=" << map->GetModel()->GetObject_(j)->GetBasicObject()->name << ", " << 
							//	character->GetName()<< "\n";
						}
					}
				}
			}

			do {
				boost::chrono::duration<float, boost::micro> dur = boost::chrono::high_resolution_clock::now() - prevTime;
				timePassed = static_cast<int>(dur.count());
			} while (timePassed < PhysicsModule::simTime);
			
		}
	}
}

void PhysicsModule::End() {
	start = false;
	run = false;
	physicsThread.join();
}
