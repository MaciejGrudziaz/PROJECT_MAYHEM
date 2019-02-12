#include "ModulesManager.h"
float ModulesManager::timeResidue = 0.0f;
float ModulesManager::lastFrameTime = 0.0f;
boost::chrono::high_resolution_clock::time_point ModulesManager::prevTime;
std::queue<float> ModulesManager::frameTime;
int ModulesManager::logFlag = 0x00;

void ModulesManager::Init() {
	logFlag |= 0x01;

	Graphics::Init();
	InputModule::Init();

	ResourcesManager::Init();

	//AutoList<Character>::GetObj(0)->StartAnimation("WALK_SIDE",70,110,Animation::NORMAL);
	//AutoList<Character>::GetObj(0)->StartAnimation(2,Animation::NORMAL);

	AutoList<BasicCharacter>::GetObj(0)->GetModel()->GetObject_(0)->Animation_SetSpeed("WALK", 4.5f);
	AutoList<BasicCharacter>::GetObj(0)->GetModel()->GetObject_(0)->Animation_SetSpeed("WALK_SIDE", 3.2f);

	StartFrameTimeMeasurment();

	PhysicsModule::Launch();
}

void ModulesManager::Process() {
	StopFrameTimeMeasurment();
	StartFrameTimeMeasurment();

	int frameCount = GetFrameCount();

	for (unsigned i = 0; i < frameCount; ++i) {
		InputModule::Process();

		for (unsigned i = 0; i < AutoList<Character>::GetCount(); ++i)
			AutoList<Character>::GetObj(i)->Update();		
	}

	if (PhysicsModule::start == false) PhysicsModule::start = true;

	Graphics::Process();
}

void ModulesManager::End() {
	Graphics::End();
	InputModule::End();
	PhysicsModule::End();

	ResourcesManager::Clear();

	if (logFlag & 0x01)
		WriteFPSToFile();
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

int ModulesManager::GetFrameCount() {
	int frameCount = lastFrameTime / CONSTVAL::targetFrameTime;
	timeResidue += lastFrameTime - static_cast<float>(frameCount)*CONSTVAL::targetFrameTime;
	if (timeResidue > CONSTVAL::targetFrameTime) {
		++frameCount;
		timeResidue -= CONSTVAL::targetFrameTime;
	}

	return frameCount;
}

void ModulesManager::StopFrameTimeMeasurment() {
	boost::chrono::duration<float, boost::micro> dur = boost::chrono::high_resolution_clock::now() - prevTime;
	lastFrameTime = dur.count() / 1000000.0;
	AddFrameTimeToQueue(lastFrameTime);
}

void ModulesManager::AddFrameTimeToQueue(float time) {
	if (frameTime.size() < 1010.0)
		frameTime.push(time);
	else {
		frameTime.pop();
		frameTime.push(time);
	}
}

void ModulesManager::WriteFPSToFile() {
	std::string content;
	int size = frameTime.size();
	int fpsTotalVal = 0;
	for (int i = 0; i < size - 10; ++i) {
		int fpsVal = static_cast<int>(1.0 / (frameTime.front()));
		fpsTotalVal += fpsVal;
		frameTime.pop();
		content += std::to_string(fpsVal);
		content += "\n";
	}
	std::fstream file("LOGS/FPS.txt", std::ios::out | std::ios::trunc);
	assert(file.is_open());
	file << "Average FPS: " << fpsTotalVal / (size - 10) << std::endl;
	file << "FPS values:\n";
	file << content;
	file.close();
}
