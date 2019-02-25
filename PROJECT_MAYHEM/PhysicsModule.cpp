#include "PhysicsModule.h"

bool PhysicsModule::start = false;
bool PhysicsModule::run=true;
boost::thread PhysicsModule::collisionDetectionThread;
boost::thread PhysicsModule::groundPosThread;

void PhysicsModule::LaunchCollisionDetection() {
	collisionDetectionThread = boost::thread(PhysicsModule(PhysicsModuleType::Collision));
}

void PhysicsModule::LaunchGroundPos() {
	groundPosThread = boost::thread(PhysicsModule(PhysicsModuleType::Ground));
}

void PhysicsModule::End() {
	start = false;
	run = false;
	collisionDetectionThread.join();
	groundPosThread.join();
}

void PhysicsModule::ProcessCollision() {
	bool col;
	boost::chrono::high_resolution_clock::time_point prevTime;
	int timePassed;

	while (run) {
		if (start) {
			prevTime = boost::chrono::high_resolution_clock::now();

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
						packet = new Packet(1, 0, 0, colData);

						CollisionDetection::GetCollisionNormals(*(AutoList<BasicCharacter>::GetObj(j)->GetModel()->GetObject_(0)->GetMainHitbox()),
							*(AutoList<BasicCharacter>::GetObj(i)->GetModel()->GetObject_(0)->GetMainHitbox()), colData->normals);

						AutoList<BasicCharacter>::GetObj(j)->PushInputPacket(packet);
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
							if(map->GetGroundObjIdx(0)!=j){
								CollisionData* colData = new CollisionData();
								Packet* packet = new Packet(1, 0, 0, colData);

								CollisionDetection::GetCollisionNormals(*(character->GetModel()->GetObject_(0)->GetMainHitbox()),
									*(map->GetModel()->GetObject_(j)->GetMainHitbox()), colData->normals);

								character->PushInputPacket(packet);
							}
						}
					}
				}
			}

			do {
				boost::chrono::duration<float, boost::micro> dur = boost::chrono::high_resolution_clock::now() - prevTime;
				timePassed = static_cast<int>(dur.count());
			} while (timePassed < CONSTVAL::targetFrameTime);
		}
	}
}

void PhysicsModule::ProcessGroundPos() {
	boost::chrono::high_resolution_clock::time_point prevTime;
	int timePassed;

	while (run) {
		if (start) {
			prevTime = boost::chrono::high_resolution_clock::now();

			for (int i = 0; i < AutoList<BasicCharacter>::GetCount(); ++i)
				GetGroundPos(AutoList<BasicCharacter>::GetObj(i), AutoList<Map>::GetObj(0));

			do {
				boost::chrono::duration<float, boost::micro> dur = boost::chrono::high_resolution_clock::now() - prevTime;
				timePassed = static_cast<int>(dur.count());
			} while (timePassed < CONSTVAL::targetFrameTime);
		}
	}
}

void PhysicsModule::GetGroundPos(BasicCharacter* character, Map* map) {
	for (int i = 0; i < map->GetGroundCount(); ++i) {
		Hitbox* characterMainHitbox = character->GetModel()->GetObject_(0)->GetMainHitbox();
		Hitbox* groundHitbox = map->GetGroundObject(i)->GetMainHitbox();

		if (CollisionDetection::CheckCollision(*characterMainHitbox, *groundHitbox) == true) {
			glm::vec3 charPos = character->GetPosition();
			glm::vec3 charPosProj;
			glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
			glm::vec3 facePt[3];
			StaticObjectTransformedVert* transVert;
			CollisionDetection::Surface surf;
			CollisionDetection::Line line;
			transVert = map->GetObjectTransformedVert(map->GetGroundObjIdx(i));
			for (int j = 0; j < map->GetModel()->GetObject_(map->GetGroundObjIdx(i))->GetBasicObject()->faces.size(); ++j) {
				for(int k=0;k<3;++k)
					facePt[k] = transVert->vertices[map->GetModel()->GetObject_(map->GetGroundObjIdx(i))->GetBasicObject()->faces[j].indices[k]];

				surf = CollisionDetection::Surface(facePt[0], facePt[1], facePt[2]);
				line = CollisionDetection::Line(yAxis, charPos);
				charPosProj = CollisionDetection::GetSurfaceCrossPoint(surf, line);

				if (CollisionDetection::IfPointInTriangle(facePt[0], facePt[1], facePt[2], charPosProj) == true) {
					SingleValData<float>* data = new SingleValData<float>(charPosProj.y);
					Packet* packet = new Packet(1, 0, 1, data);

					character->PushInputPacket(packet);
					break;
				}
			}
		}
	}
}
