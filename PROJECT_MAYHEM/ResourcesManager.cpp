#include "ResourcesManager.h"

void ResourcesManager::Init() {
	//BasicCharacter* soldier=LoadModel<BasicCharacter>("MODELS/TEST_1_4.mgr","MODEL1");
	Player* soldier = LoadModel<Player>("MODELS/TEST_1_4.mgr", "MODEL1");
	AutoList<Player>::GetObj(0)->SetPosition(glm::vec3(-10.0f, 0.0f, 25.0f));
	

	//glm::vec3 pos = AutoList<BasicCharacter>::GetObj(0)->GetPosition();
	//pos.z -= 10.0f;
	//glm::vec3 size = AutoList<BasicCharacter>::GetObj(0)->GetSize();
	//pos.y += size.y;
	//new Camera(pos, glm::vec3(0.0f, -0.2f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	FollowCamera* cam=new FollowCamera();
	cam->SetFollowCharacter(soldier);
	cam->Activate();

	LoadModel<BasicCharacter>("MODELS/TEST_1_4.mgr", "MODEL2");
	AutoList<BasicCharacter>::GetObj(1)->StartAnimation("WAVING", Animation::NORMAL);
	AutoList<BasicCharacter>::GetObj(1)->SetPosition(glm::vec3(0.0f, 0.0f, 25.0f));

	Map* map=LoadModel<Map>("MODELS/MAP_1.mgr","MAP");
	//map->Scale(10.0f, 10.0f, 10.0f);
	map->TransformObjectsVertices();
	map->SetGroundObject("Plane");
}

void ResourcesManager::Clear() {
	AutoList<BasicCharacter>::ClearAll();
	AutoList<Camera>::ClearAll();
}

//---------------------------------------------------------------------------------------
//----------------------------------------PRIVATE----------------------------------------
//---------------------------------------------------------------------------------------

template <class T>
T* ResourcesManager::LoadModel(const char* filename,const char* objectName) {
	Shader *modelStatic, *modelDynamic, *hitboxStatic, *hitboxDynamic, *hitboxComputeDynamic, *hitboxComputeStatic;
	modelStatic = AutoMap<Shader>::GetObj("StaticObj");
	modelDynamic = AutoMap<Shader>::GetObj("DynamicObj");
	hitboxStatic = AutoMap<Shader>::GetObj("StaticHitbox");
	hitboxDynamic = AutoMap<Shader>::GetObj("DynamicHitbox");

	//if (AutoList<Character>::GetCount() > 0)
	//	AutoList<Character>::DestoryObj(0);
	//if (AutoList<BasicModel>::GetCount() > 0)
	//	AutoList<BasicModel>::DestoryObj(0);

	bool isDynamic = false;

	BasicModel* basicModel = new BasicModel();
	ImportFile::Import(filename, basicModel);


	if (basicModel != nullptr) {
		//BasicCharacter* character = new BasicCharacter("TEST");
		T* character = new T(objectName);
		//Character* character = new Character("TEST");
		character->InitModel(basicModel);

		std::string filenameStr(filename);
		for (int i = 0; i < character->GetObjectsCount(); ++i) {
			if (character->IsObjectDynamic(i)) {
				character->LoadObjectShaderProgram(i, modelDynamic);
				character->LoadObjectHitboxShaderProgram(i, hitboxDynamic);
			}
			else {
				character->LoadObjectShaderProgram(i, modelStatic);
				character->LoadObjectHitboxShaderProgram(i, hitboxStatic);
			}
			if (strcmp(filename + (filenameStr.size() - 4), ".mgr") == 0) {
				character->Scale(ImportFile::GetScaleMat());
				character->GetModel()->GetObject_(i)->AddMainHitbox(ImportFile::GetMainHitbox(i));
				if (ImportFile::ImportedHitboxesAvailableForObject(i) == true) {
					for (int j = 0; j < ImportFile::GetHitboxCount(i); ++j) {
						Hitbox* hitbox = ImportFile::GetHitbox(i, j);
						if (hitbox != nullptr)
							character->GetModel()->GetObject_(i)->AddHitbox(hitbox);
					}
				}
				character->GetModel()->GetObject_(i)->LoadHitboxBufferData();
			}
		}
		character->Init();

		character->SetProjectionMatrix(WinAPIwindowManager::GetMainWindow()->GetProjectionMatrix());
		character->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		character->SetSpeed(20.f);
		character->SetRotationSpeed(0.5f);

		return character;
	}

	return nullptr;
}

void ResourcesManager::DeleteModel(int idx) {
	if (idx >= 0 && idx < AutoList<Character>::GetCount())
		AutoList<Character>::DestoryObj(idx);
}
