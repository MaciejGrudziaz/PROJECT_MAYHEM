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
	//std::vector<glm::vec3> collisionNormals;
	boost::chrono::high_resolution_clock::time_point prevTime;
	int timePassed;

	while (run) {
		if (start) {
			prevTime = boost::chrono::high_resolution_clock::now();

			//collisionNormals.clear();
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
							/*if (map->GetModel()->GetObject_(j)->GetBasicObject()->name != map->GetGroundObject(0)->GetBasicObject()->name) {*/
							if(map->GetGroundObjIdx(0)!=j){
								CollisionData* colData = new CollisionData();
								Packet* packet = new Packet(1, 0, 0, colData);

								CollisionDetection::GetCollisionNormals(*(character->GetModel()->GetObject_(0)->GetMainHitbox()),
									*(map->GetModel()->GetObject_(j)->GetMainHitbox()), colData->normals);

								character->PushInputPacket(packet);
							}
							//else {
							//	//GroundCollision* colData = new GroundCollision(0.0f);
							//	//Packet* packet = new Packet(1, 0, 1, colData);

							//	//character->PushInputPacket(packet);
							//}

							//std::cout << "Collision: map obj name=" << map->GetModel()->GetObject_(j)->GetBasicObject()->name << ", " << 
							//	character->GetName()<< "\n";
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
	int cnt1, cnt2;

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

//void PhysicsModule::GetGroundPos(BasicCharacter* character, Map* map) {
//	struct GroundVertDist {
//		glm::vec3 vert;
//		float dist;
//	};
//
//	for (int i = 0; i < map->GetGroundCount(); ++i) {
//		Hitbox* characterMainHitbox = character->GetModel()->GetObject_(0)->GetMainHitbox();
//		Hitbox* groundHitbox = map->GetGroundObject(i)->GetMainHitbox();
//		if (CollisionDetection::CheckCollision(*characterMainHitbox, *groundHitbox) == true) {
//			glm::vec3 charPosProj = character->GetPosition();
//			charPosProj.y = 0.0f;
//			GroundVertDist groundClosestPoints[4];
//			StaticObjectTransformedVert* groundVert = map->GetObjectTransformedVert(map->GetGroundObjIdx(i));
//			GroundVertDist tmpStruct;
//			glm::vec3 projVert;
//
//			if (groundVert->vertices.size() > 3) {
//				for (int j = 0; j < 4; ++j) {
//					groundClosestPoints[j].vert = groundVert->vertices[j];
//					groundClosestPoints[j].dist = glm::distance(glm::vec3(groundClosestPoints[j].vert.x, 0.0f, groundClosestPoints[j].vert.z), charPosProj);
//				}
//
//				for (int j = 0; j < 3; ++j) {
//					for (int k = j + 1; k < 4; ++k) {
//						if (groundClosestPoints[k].dist < groundClosestPoints[j].dist) {
//							tmpStruct = groundClosestPoints[k];
//							groundClosestPoints[k] = groundClosestPoints[j];
//							groundClosestPoints[j] = tmpStruct;
//						}
//					}
//				}
//
//				for (int j = 4; j < groundVert->vertices.size(); ++j) {
//					tmpStruct.vert = groundVert->vertices[j];
//					tmpStruct.dist = glm::distance(glm::vec3(tmpStruct.vert.x,0.0f, tmpStruct.vert.z), charPosProj);
//					for (int k = 0; k < 4; ++k) {
//						if (tmpStruct.dist < groundClosestPoints[k].dist) {
//							for (int l = 3; l >k; --l) {
//								groundClosestPoints[l] = groundClosestPoints[l - 1];
//							}
//							groundClosestPoints[k] = tmpStruct;
//							break;
//						}
//					}
//				}
//
//				
//				glm::vec3 norm = glm::cross((groundClosestPoints[1].vert - groundClosestPoints[0].vert), (groundClosestPoints[2].vert - groundClosestPoints[0].vert));
//				if (glm::dot(norm, glm::vec3(0.0f, 1.0f, 0.0f)) < 0.0f) norm = -norm;
//				norm = glm::normalize(norm);
//				CollisionDetection::Surface surf(norm, groundClosestPoints[0].vert);
//				CollisionDetection::Line line(glm::vec3(0.0f, 1.0f, 0.0f), charPosProj);
//
//				glm::vec3 groundPtProj = CollisionDetection::GetSurfaceCrossPoint(surf, line);
//
//				SingleValData<float> *data = new SingleValData<float>(groundPtProj.y);
//				Packet* packet = new Packet(1, 0, 1, data);
//
//				character->PushInputPacket(packet);
//			}
//		}
//	}
//}
