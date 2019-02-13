#include "BasicCharacter.h"

void BasicCharacter::Init() { 
	moveAnimNames.insert(AnimNameMap::value_type("WALK", 3));
	moveAnimNames.insert(AnimNameMap::value_type("WALK_SIDE", 4));
	moveAnimNames.insert(AnimNameMap::value_type("RIGHT_WALK", 2));
	moveAnimNames.insert(AnimNameMap::value_type("LEFT_WALK", 0));

	Character::Init(); 
}

void BasicCharacter::Update() { 
	ProcessPackets();

	Character::Update(); 
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void BasicCharacter::ProcessPackets() {
	int dir1=0, dir2=0;
	float rotSpeed = GetRotationSpeed();
	glm::vec3 rotUpdate(0.0f);
	Packet* p;
	std::vector<Packet*> inputPackets;

	GetAllInputPackets_priority(inputPackets);

	for (int i = 0; i < inputPackets.size(); ++i) {
		p = inputPackets[i];
		if (p->class_ == 0) {
			if (p->group_ == 0) {
				if (p->id_ == 1) dir1 += 1;
				else if (p->id_ == 2) dir1 -= 1;
				else if (p->id_ == 3) dir2 += 1;
				else if (p->id_ == 4) dir2 -= 1;
			}
			else if (p->group_ == 1) {
				//int moveVal = static_cast<MouseMoveData*>(p->data)->moveVal;
				int moveVal = p->GetData<MouseMoveData>()->moveVal;

				if (p->id_ == 1)
					rotUpdate.x += (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 2)
					rotUpdate.x -= (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 3)
					rotUpdate.y += (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
				else if (p->id_ == 4)
					rotUpdate.y -= (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
			}
			delete p;
			p = nullptr;
		}		
	}

	glm::vec3 changeVec(0.0f);
	std::vector<glm::vec3> *normalsVec;
	for (int i = 0; i < inputPackets.size(); ++i) {
		p = inputPackets[i];
		if (p != nullptr) {
			if (p->class_ == 1) {
				if (p->id_ == 0) {
					normalsVec = &(p->GetData<CollisionData>()->normals);
					for (int j = 0; j < normalsVec->size(); ++j)
						changeVec += normalsVec->operator[](j);
				}
				delete p;
				p = nullptr;
			}
		}
	}

	//for (int i = 0; i < inputPackets.size(); ++i)
	//	if (inputPackets[i] != nullptr) delete inputPackets[i];

	//while (InputPacketsAvailable()) {
	//	p = PopInputPacket();
	//	if (p->class_ == 0) {
	//		if (p->group_ == 0) {
	//			if (p->id_ == 1) dir1 += 1;
	//			else if (p->id_ == 2) dir1 -= 1;
	//			else if (p->id_ == 3) dir2 += 1;
	//			else if (p->id_ == 4) dir2 -= 1;
	//		}
	//		else if (p->group_ == 1) {
	//			//int moveVal = static_cast<MouseMoveData*>(p->data)->moveVal;
	//			int moveVal = p->GetData<MouseMoveData>()->moveVal;

	//			if (p->id_ == 1)
	//				rotUpdate.x += (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
	//			else if (p->id_ == 2)
	//				rotUpdate.x -= (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
	//			else if (p->id_ == 3)
	//				rotUpdate.y += (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
	//			else if (p->id_ == 4)
	//				rotUpdate.y -= (rotSpeed*CONSTVAL::targetFrameTime*moveVal);
	//		}
	//	}
	//	else if (p->class_ == 1) {
	//		
	//	}
	//	delete p;
	//}

	UpdateRotation(rotUpdate);

	MoveAction(dir1, dir2);

	glm::vec3 moveVec= nextPos - prevPos;;
	float length;
	if (changeVec != glm::vec3(0.0f) && moveVec!=glm::vec3(0.0f)) {
		changeVec = glm::normalize(changeVec);
		moveVec = nextPos - prevPos;
		length=glm::length(moveVec);
		moveVec = glm::normalize(moveVec);
		moveVec = moveVec + changeVec;
		moveVec = length * glm::normalize(moveVec);
		nextPos = prevPos + moveVec;
	}
	//moveVec = nextPos - prevPos;
	//moveVec = GetSpeed()*glm::normalize(nextPos*prevPos);
	//nextPos = prevPos + moveVec;
	SetPosition(nextPos);

	glm::quat q, qx, qy;
	glm::mat3 rot;
	glm::vec3 rotations = GetRotations(),lookVec;
	float sx = sin(rotations.x / 2.0f), sy = sin(-rotations.y / 2.0f);
	qx.w = cos(rotations.x / 2.0f);
	qx.x = sx * 1.0f;

	qy.w = cos(-rotations.y / 2.0f);
	qy.y = sy * 1.0f;

	q = glm::cross(qx, qy);
	rot = glm::mat3_cast(q);
	lookVec = glm::vec3(0.0f, 0.0f, 1.0f) * rot;
	SetLookAtVec(lookVec);

	//glm::vec3 cameraPos = GetPosition();
	//cameraPos.y += GetSize().y;
	//cameraPos -= (12.0f*lookVec);
	//AutoList<Camera>::GetObj(0)->SetPosition(cameraPos);
	//AutoList<Camera>::GetObj(0)->SetLookVec(lookVec);
}

void BasicCharacter::MoveAction(int dir1, int dir2) {
	if (dir1 != 0 || dir2 != 0) {
		if (dir1 == 0) {
			for (AnimNameMap::iterator it = moveAnimNames.begin(); it != moveAnimNames.end(); ++it) {
				if (it->first == "WALK" || it->first == "LEFT_WALK" || it->first == "RIGHT_WALK") {
					if (IsAnimationActive(it->second) && GetAnimationIterationsLeft(it->second)!=1){
						if(it->first=="WALK") StartAnimation(it->second, 0, 20, 1, Animation::REVERSE);
						else StartAnimation(it->second, 0, 10, 1, Animation::REVERSE);
					}
					//if (IsAnimationActive(it->second) && GetAnimationIterationsLeft(it->second) == 1 && GetAnimationCurrentFrame(it->second) == 1)
					//	StopAnimations(it->second);
				}
			}

			int idx = moveAnimNames.find("WALK_SIDE")->second;
			if (dir2 == 1) {
				if (!IsAnimationActive(idx))
					StartAnimation(idx, 70, Animation::MAX_FRAME, Animation::REVERSE);
			}
			else {
				if (!IsAnimationActive(idx))
					StartAnimation(idx, 70, Animation::MAX_FRAME, Animation::NORMAL);
			}
		}
		else if (dir2 == 0) {
			for (AnimNameMap::iterator it = moveAnimNames.begin(); it != moveAnimNames.end(); ++it) {
				if (it->first == "WALK_SIDE" || it->first == "LEFT_WALK" || it->first == "RIGHT_WALK") {
					if (IsAnimationActive(it->second) && GetAnimationIterationsLeft(it->second)!=1) {
						if (it->first == "WALK_SIDE") StartAnimation(it->second, 0, 20, 1, Animation::REVERSE);
						else StartAnimation(it->second, 0, 10, 1, Animation::REVERSE);
					}
					//if (IsAnimationActive(it->second) && GetAnimationIterationsLeft(it->second) == 1 && GetAnimationCurrentFrame(it->second) == 1)
					//	StopAnimations(it->second);
				}
			}

			int idx = moveAnimNames.find("WALK")->second;
			if (dir1 == 1) {
				if (!IsAnimationActive(idx))
					StartAnimation(idx, 20, Animation::MAX_FRAME, Animation::NORMAL);
			}
			else {
				if (!IsAnimationActive(idx))
					StartAnimation(idx, 20, Animation::MAX_FRAME, Animation::REVERSE);
			}
		}
		else {
			int idx_ws = moveAnimNames.find("WALK_SIDE")->second, idx_w = moveAnimNames.find("WALK")->second, 
				idx_rw = moveAnimNames.find("RIGHT_WALK")->second, idx_lw = moveAnimNames.find("LEFT_WALK")->second;
			
			if (IsAnimationActive(idx_ws) && GetAnimationIterationsLeft(idx_ws) != 1){
				StopAnimations(idx_ws);
				StartAnimation(idx_ws, 0, 20, 1, Animation::REVERSE);
			}

			if (dir1 == 1) {
				if (dir2 == -1) {
					if(IsAnimationActive(idx_rw) && GetAnimationCurrentFrame(idx_rw)==9)
						StartAnimation(idx_rw, 10, Animation::MAX_FRAME, Animation::NORMAL);
					else if (!IsAnimationActive(idx_rw))
						StartAnimation(idx_rw, 0, 10, 1, Animation::NORMAL);
				}
				else {
					if (IsAnimationActive(idx_lw) && GetAnimationCurrentFrame(idx_lw) == 9)
						StartAnimation(idx_lw, 10, Animation::MAX_FRAME, Animation::NORMAL);
					else if (!IsAnimationActive(idx_lw))
						StartAnimation(idx_lw, 0, 10, 1, Animation::NORMAL);
				}

				if(!IsAnimationActive(idx_w))
					StartAnimation(idx_w, 20, Animation::MAX_FRAME, Animation::NORMAL);	
			}
			else {
				if (dir2 == 1) {
					if (IsAnimationActive(idx_rw) && GetAnimationCurrentFrame(idx_rw) == 9)
						StartAnimation(idx_rw, 10, Animation::MAX_FRAME, Animation::NORMAL);
					else if (!IsAnimationActive(idx_rw))
						StartAnimation(idx_rw, 0, 10, 1, Animation::NORMAL);
				}
				else {
					if (IsAnimationActive(idx_lw) && GetAnimationCurrentFrame(idx_lw) == 9)
						StartAnimation(idx_lw, 10, Animation::MAX_FRAME, Animation::NORMAL);
					else if (!IsAnimationActive(idx_lw))
						StartAnimation(idx_lw, 0, 10, 1, Animation::NORMAL);
				}
				if (!IsAnimationActive(idx_w))
					StartAnimation(idx_w, 20, Animation::MAX_FRAME, Animation::REVERSE);
			}
		}
	}
	else {
		for (AnimNameMap::iterator it = moveAnimNames.begin(); it != moveAnimNames.end(); ++it) {
			if (IsAnimationActive(it->second) && (GetAnimationIterationsLeft(it->second))!=1){
				if(it->first=="WALK") StartAnimation(it->second, 0, 20, 1, Animation::REVERSE);
				else if (it->first == "WALK_SIDE") {
					if(GetAnimationCurrentFrame(it->second)<90)
						StartAnimation(it->second, Animation::CURR_FRAME, 90, 1, Animation::NORMAL);
					else StartAnimation(it->second, Animation::CURR_FRAME, Animation::MAX_FRAME, 1, Animation::NORMAL);
				}
				else StartAnimation(it->second, 0, 10, 1, Animation::REVERSE);
			}
		}
	}

	float speed = GetSpeed();
	glm::vec3 pos = GetPosition();
	glm::vec3 lookVec = GetLookAtVec();

	prevPos = pos;

	if (dir1 != 0 && dir2 != 0) speed /= 2.0f;

	if(dir1==1) pos += (speed*CONSTVAL::targetFrameTime)*glm::normalize(glm::vec3(lookVec.x, 0.0f, lookVec.z));
	else if(dir1==-1) pos -= (speed*CONSTVAL::targetFrameTime)*glm::normalize(glm::vec3(lookVec.x, 0.0f, lookVec.z));

	if(dir2==1) pos += (speed*CONSTVAL::targetFrameTime)*(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(glm::vec3(lookVec.x, 0.0f, lookVec.z))));
	else if(dir2==-1) pos -= (speed*CONSTVAL::targetFrameTime)*(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(glm::vec3(lookVec.x, 0.0f, lookVec.z))));

	nextPos = pos;

	//SetPosition(pos);
}
