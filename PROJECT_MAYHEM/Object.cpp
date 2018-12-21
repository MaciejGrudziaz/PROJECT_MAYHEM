#include "Object.h"

//-----------------------------------------------------------------------------
//-----------------------------------Object------------------------------------
//-----------------------------------------------------------------------------

Object::Object() :vertices(nullptr), hitboxVertices(nullptr), basicObject(nullptr), objectBufferVertexAttribCount(13),hitboxObjectBufferVertexAttribCount(4)
{ }

void Object::LoadHitboxDataFromShaders() {
	static int unsigned test = 0;
	//if (basicObject->name == "Plane")
		++test;

	int hitboxBufferIdx = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 3; ++j) {
			mainHitbox->transformVertices[i][j] = hitboxOutVertices[hitboxBufferIdx];
			++hitboxBufferIdx;
		}
		++hitboxBufferIdx;
	}

	for (HitboxMap::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 3; ++j) {
				it->second->transformVertices[i][j] = hitboxOutVertices[hitboxBufferIdx];
				++hitboxBufferIdx;
			}
			++hitboxBufferIdx;
		}
	}
}

void Object::LoadShader(Shader* shaderProgram_) { 
	if (shaderProgram_ != nullptr)
		shaderManager->LoadMainShaderProgram(shaderProgram_);
}

void Object::LoadHitboxShader(Shader* hitboxBasicShaderProgram_) {
	if (hitboxBasicShaderProgram_ != nullptr)
		shaderManager->LoadHitboxVisualizationShaderProgram(hitboxBasicShaderProgram_);
	else throw std::exception();
	
}

void Object::LoadHitboxComputeShader(Shader* hitboxComputeShaderProgram) {
	if (hitboxComputeShaderProgram != nullptr)
		shaderManager->LoadHitboxComputeShaderProgram(hitboxComputeShaderProgram);
	else throw std::exception();
}

int Object::LoadTexture(std::string filename) {
	if (filename == g_modelsDirectory) {
		textureAvailable = 0;
		shaderManager->LoadTexture("");
		return 1;
	}
	else {
		textureAvailable = 1;
		shaderManager->LoadTexture(filename.c_str());
		return 1;
	}
}

int Object::StartAnimation(int animationIndex_, int direction) {
	if (animationManager == nullptr) return -1;
	else if (animationIndex_<0 || animationIndex_ > animationManager->GetAnimationsCount() - 1)
		return 0;
	else {
		animationManager->Start(animationIndex_,direction);
		return 1;
	}
}

int Object::StartAnimation(int animationIndex_, int startFrame, int stopFrame, int direction) {
	if (animationManager == nullptr) return -1;
	else if (animationIndex_<0 || animationIndex_ > animationManager->GetAnimationsCount() - 1)
		return 0;
	else {
		animationManager->Start(animationIndex_,startFrame,stopFrame,direction);
		return 1;
	}
}

int Object::StartAnimation(int animationIndex_, int startFrame, int stopFrame, int iterations, int direction) {
	if (animationManager == nullptr) return -1;
	else if (animationIndex_<0 || animationIndex_ > animationManager->GetAnimationsCount() - 1)
		return 0;
	else {
		animationManager->Start(animationIndex_, startFrame, stopFrame,iterations,direction);
		return 1;
	}
}

int Object::StartAnimation(const char* name, int direction) {
	if (animationManager == nullptr) return -1;
	else {
		animationManager->Start(name,direction);
		return 1;
	}
}

int Object::StartAnimation(const char* name, int startFrame, int stopFrame, int direction) {
	if (animationManager == nullptr) return -1;
	else {
		animationManager->Start(name,startFrame,stopFrame,direction);
		return 1;
	}
}

int Object::StartAnimation(const char* name, int startFrame, int stopFrame, int iterations, int direction) {
	if (animationManager == nullptr) return -1;
	else {
		animationManager->Start(name,startFrame,stopFrame,iterations,direction);
		return 1;
	}
}

int Object::StopAnimation(int animationIndex_) {
	if (animationManager == nullptr) return -1;
	else if (animationIndex_<0 || animationIndex_> animationManager->GetAnimationsCount() - 1)
		return 0;
	else {
		animationManager->Stop(animationIndex_);
		return 1;
	}
}

int Object::StopAnimation(const char* name) {
	if (animationManager == nullptr) return -1;
	else {
		animationManager->Stop(name);
		return 1;
	}
}

int Object::StopAllAnimations() {
	if (animationManager == nullptr) return -1;
	else {
		animationManager->StopAll();
		return 1;
	}
}

bool Object::IsAnimationActive(int animationIndex) {
	if (animationManager == nullptr) return false;
	else return animationManager->IsActive(animationIndex);
}

bool Object::IsAnimationActive(const char* name) {
	if (animationManager == nullptr) return false;
	else return animationManager->IsActive(name);
}

int Object::GetAnimationIterationsLeft(int animationIndex) {
	if (animationManager == nullptr) return -1;
	else return animationManager->GetIterationsLeft(animationIndex);
}

int Object::GetAnimationIterationsLeft(const char* name) {
	if (animationManager == nullptr) return -1;
	else return animationManager->GetIterationsLeft(name);
}

int Object::GetAnimationCurrentFrame(int animationIndex) {
	if (animationManager == nullptr) return -1;
	else return animationManager->GetCurrentFrame(animationIndex);
}

int Object::GetAnimationCurrentFrame(const char* name) {
	if (animationManager == nullptr) return -1;
	else return animationManager->GetCurrentFrame(name);
}

int Object::GetAnimationDirection(int animationIndex) {
	if (animationManager == nullptr) return -2;
	else return animationManager->GetDirection(animationIndex);
}

int Object::GetAnimationDirection(const char* name) {
	if (animationManager == nullptr) return -2;
	else return animationManager->GetDirection(name);
}

Animation* Object::GetAnimation(int index) {
	if (animationManager != nullptr) {
		if (index >= 0 && index < animationManager->GetAnimationsCount())
			return animationManager->GetAnimation(index);
		else return nullptr;
	}
	else return nullptr;
}

Animation* Object::GetAnimation(const char* name) {
	if (animationManager != nullptr) {
		for (int i = 0; i < animationManager->GetAnimationsCount(); ++i) {
			if (animationManager->GetAnimation(i)->name == name) {
				return animationManager->GetAnimation(i);
			}
		}
		return nullptr;
	}
	else return nullptr;
}

int Object::Animation_GetSpeed(int index) {
	if (animationManager != nullptr) {
		if (index >= 0 && index < animationManager->GetAnimationsCount())
			return animationManager->GetAnimation(index)->speed;
		else return -2;
	}
	else return -1;
}

int Object::Animation_GetSpeed(const char* name) {
	if (animationManager != nullptr) {
		for (int i = 0; i < animationManager->GetAnimationsCount(); ++i) {
			if (animationManager->GetAnimation(i)->name == name) {
				return animationManager->GetAnimation(i)->speed;
			}
		}
		return -2;
	}
	else return -1;
}

int Object::Animation_SetSpeed(int index, float speed) {
	if (animationManager != nullptr) {
		if (index >= 0 && index < animationManager->GetAnimationsCount()) {
			animationManager->GetAnimation(index)->speed = speed;
			return 0;
		}
		else return -2;
	}
	else return -1;
}

int Object::Animation_SetSpeed(const char* name, float speed) {
	if (animationManager != nullptr) {
		for (int i = 0; i < animationManager->GetAnimationsCount(); ++i) {
			if (animationManager->GetAnimation(i)->name == name) {
				animationManager->GetAnimation(i)->speed=speed;
				return 0;
			}
		}
		return -2;
	}
	else return -1;
}

const char* Object::Animation_GetName(int index) {
	if (animationManager != nullptr) {
		if (index >= 0 && index < animationManager->GetAnimationsCount())
			return animationManager->GetAnimation(index)->name.c_str();
		else return "";
	}
	else return "";
}

int Object::Animation_SetName(int index, const char* name) {
	if (animationManager != nullptr) {
		if (index >= 0 && index < animationManager->GetAnimationsCount()) {
			animationManager->GetAnimation(index)->name = name;
			return 0;
		}
		else return -2;
	}
	else return -1;
}

//-----------------------------------------------------------------------------
//-------------------------------Object::Private-------------------------------
//-----------------------------------------------------------------------------

void Object::LoadHitboxBufferData() {
	hitboxVerticesCount = (hitboxes.size()+1) * 8 * (3+1);

	hitboxVertices = new float[hitboxVerticesCount];
	hitboxOutVertices = new float[hitboxVerticesCount];

	for (int j = 0; j < 8; ++j) {
		for (int k = 0; k < 3; ++k)
			hitboxVertices[j * 4 + k] = mainHitbox->basicVertices[j][k];
		hitboxVertices[j * 4 + 3] = static_cast<float>(mainHitbox->jointIdx);
	}

	int i = 1;
	for (HitboxMap::iterator it = hitboxes.begin(); it != hitboxes.end();++it) {
		for (unsigned j = 0; j < 8; ++j) {
			for (unsigned k = 0; k < 3; ++k)
				hitboxVertices[i * 32 + j * 4 + k] = it->second->basicVertices[j][k];
			hitboxVertices[i * 32 + j * 4 + 3] = static_cast<float>(it->second->jointIdx);
		}
		++i;
	}
}

void Object::WriteErrorToFile(std::string message) {
	std::fstream file("LOGS/" + basicObject->name + "_ERRORLOG.txt", std::ios::out | std::ios::app);
	assert(file.is_open());
	SYSTEMTIME time;
	GetSystemTime(&time);
	file << time.wDay << "." << time.wMonth << "." << time.wYear << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << " - ";
	file << message << std::endl;
	file.close();
}

void Object::GetSizeFromMainHitbox() {
	minVec = maxVec = basicObject->globalTransform*mainHitbox->basicVertices[0];
	
	glm::vec3 vert;
	for (int i = 1; i < 8; ++i) {
		vert = basicObject->globalTransform*mainHitbox->basicVertices[i];

		for (int j = 0; j < 3; ++j) {
			if (vert[j] > maxVec[j]) maxVec[j] = vert[j];
			if (vert[j] < minVec[j]) minVec[j] = vert[j];
		}
	}

	for (int i = 0; i < 3; ++i)
		size[i] = maxVec[i] - minVec[i];
}

//-----------------------------------------------------------------------------
//--------------------------------DynamicObject--------------------------------
//-----------------------------------------------------------------------------

void DynamicObject::Init() {
	objectBufferVertexAttribCount = 13;
	hitboxObjectBufferVertexAttribCount = 4;

	animationManager = new AnimationManager(basicObject);

	SetVerticesBuffer();

	shaderManager->SetObjectBufferVertexAttribCount(objectBufferVertexAttribCount);
	shaderManager->SetHitboxObjectBufferVertexAttribCount(hitboxObjectBufferVertexAttribCount);
	shaderManager->Init();
	shaderManager->SetVAO(vertices, verticesCount);
	shaderManager->SetHitboxVAO(hitboxVertices, Hitbox::indices, hitboxVerticesCount);
	shaderManager->SetHitboxComputeBuffers(hitboxVertices, hitboxVerticesCount);
	shaderManager->SetJointsBuffers(animationManager->GetJointsMatricesCount());
	
	//glUseProgram(shaderManager->GetHitboxShader()->GetProgram());

	memcpy(shaderManager->GetJointsPrevMatBufferPtr(), animationManager->GetJointsPreviousTransformMatrices(), animationManager->GetJointsMatricesCount() * 16 * sizeof(float));
	memcpy(shaderManager->GetJointsNextMatBufferPtr(), animationManager->GetJointsNextTransformMatrices(), animationManager->GetJointsMatricesCount() * 16 * sizeof(float));

	if (!LoadTexture(g_modelsDirectory + basicObject->textureName)) {
		WriteErrorToFile("Error loading texture!");
		WriteErrorToFile("Texture name: " + basicObject->textureName);
		throw std::exception();
	}
}

void DynamicObject::Update() {
	animationManager->Process();

	//if (modelUpdate || viewUpdate || projectionUpdate) {
		//if (modelUpdate)
			model = model * basicObject->globalTransform;

		glUseProgram(shaderManager->GetMainShader()->GetProgram());
		//if (modelUpdate) 
			glUniformMatrix4fv(shaderManager->GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));
		//if (viewUpdate)
			glUniformMatrix4fv(shaderManager->GetViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
		//if (projectionUpdate)
			glUniformMatrix4fv(shaderManager->GetProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

		glUseProgram(shaderManager->GetHitboxShader()->GetProgram());
		//if (modelUpdate) 
			glUniformMatrix4fv(shaderManager->GetModelHitboxComputeLoc(), 1, GL_FALSE, glm::value_ptr(model));
		//if (viewUpdate) 
			glUniformMatrix4fv(shaderManager->GetViewHitboxLoc(), 1, GL_FALSE, glm::value_ptr(view));
		//if (projectionUpdate) 
			glUniformMatrix4fv(shaderManager->GetProjectionHitboxLoc(), 1, GL_FALSE, glm::value_ptr(projection));

		//modelUpdate = viewUpdate = projectionUpdate = false;
	//}

	if (animationManager->GetJointsMatChangeStatus() == true) {
		memcpy(shaderManager->GetJointsPrevMatBufferPtr(), animationManager->GetJointsPreviousTransformMatrices(), animationManager->GetJointsMatricesCount() * 16 * sizeof(float));
		memcpy(shaderManager->GetJointsNextMatBufferPtr(), animationManager->GetJointsNextTransformMatrices(), animationManager->GetJointsMatricesCount() * 16 * sizeof(float));
	}

	UpdateHitboxes();
}

void DynamicObject::Draw() {
	unsigned shaderProgram = shaderManager->GetMainShader()->GetProgram();
	int interpolatioLoc = shaderManager->GetInterpolationLoc();
	int modelLoc = shaderManager->GetModelLoc();
	int texAvailableLoc = shaderManager->GetTextureAvailableLoc();
	unsigned texture = shaderManager->GetTexture();
	unsigned VAO = shaderManager->GetVAO();
	unsigned prevMatBuffer = shaderManager->GetJointsPrevMatricesBuffer();
	unsigned nextMatBuffer = shaderManager->GetJointsNextMatricesBuffer();
	unsigned hitboxShaderProgram = shaderManager->GetHitboxShader()->GetProgram();
	int hitboxInterpolationLoc = shaderManager->GetHitboxInterpolationLoc();
	int hitboxModelLoc = shaderManager->GetModelHitboxLoc();
	unsigned hitboxVAO = shaderManager->GetHitboxVAO();

	unsigned jointsCount = animationManager->GetJointsMatricesCount();

	glUseProgram(shaderProgram);
	glUniform1f(interpolatioLoc, animationManager->GetInterpolationVal());
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(texAvailableLoc, textureAvailable);

	if (textureAvailable) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, prevMatBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, nextMatBuffer);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount / objectBufferVertexAttribCount);

	glUseProgram(hitboxShaderProgram);

	glUniform1f(hitboxInterpolationLoc, animationManager->GetInterpolationVal());
	glUniformMatrix4fv(hitboxModelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(hitboxVAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, prevMatBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, nextMatBuffer);

	glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 0);

	int i = 1;
	for (HitboxMap::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		int startVal = 8 * i;
		glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, startVal);
		++i;
	}
}

//-----------------------------------------------------------------------------
//---------------------------DynamicObject::Private----------------------------
//-----------------------------------------------------------------------------

void DynamicObject::SetVerticesBuffer() {
	verticesCount = basicObject->faces.size() * 3 * 13;
	vertices = new float[verticesCount];
	for (unsigned i = 0; i < basicObject->faces.size(); ++i) {
		for (unsigned j = 0; j < 3; ++j) {
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 0] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.x;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 1] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.y;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 2] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.z;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 3] = basicObject->faces[i].uv[j].x;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 4] = basicObject->faces[i].uv[j].y;
			for (unsigned k = 0; k < 4; ++k) {
				vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 2 * k + 5] = basicObject->controlPoints[basicObject->faces[i].indices[j]].blendingInfo[k].jointIndex;
				vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 2 * k + 1 + 5] = basicObject->controlPoints[basicObject->faces[i].indices[j]].blendingInfo[k].weight;
			}
		}
	}
}

void DynamicObject::UpdateHitboxes() {
	unsigned computeShaderProgram = shaderManager->GetHitboxComputeShader()->GetProgram();
	int modelLoc = shaderManager->GetModelHitboxComputeLoc();
	int interpolationLoc = shaderManager->GetHitboxComputeInterpolationLoc();
	unsigned computeInBuffer = shaderManager->GetHitboxComputeInBuffer();
	unsigned computeOutBuffer = shaderManager->GetHitboxComputeOutBuffer();
	unsigned prevMatBuffer = shaderManager->GetJointsPrevMatricesBuffer();
	unsigned nextMatBuffer = shaderManager->GetJointsNextMatricesBuffer();

	if (updateHitboxVerticesInBuffer != 0) {
		glNamedBufferSubData(shaderManager->GetHitboxVBO(), 0, hitboxVerticesCount * sizeof(float), hitboxVertices);
		glNamedBufferSubData(shaderManager->GetHitboxComputeInBuffer(), 0, hitboxVerticesCount * sizeof(float), hitboxVertices);
		updateHitboxVerticesInBuffer = 0;
	}

	glUseProgram(computeShaderProgram);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(interpolationLoc, animationManager->GetInterpolationVal());

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeInBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, computeOutBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, prevMatBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, nextMatBuffer);

	glDispatchCompute(hitboxes.size()+1, 1, 1);

	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);

	memcpy(hitboxOutVertices, shaderManager->GetHitboxComputeOutBuferPtr(), hitboxVerticesCount * sizeof(float));

	LoadHitboxDataFromShaders();
}

//-----------------------------------------------------------------------------
//--------------------------------StaticObject---------------------------------
//-----------------------------------------------------------------------------

void StaticObject::Init() {
	objectBufferVertexAttribCount = 5;
	hitboxObjectBufferVertexAttribCount = 4;

	animationManager = nullptr;

	SetVerticesBuffer();

	shaderManager->SetObjectBufferVertexAttribCount(objectBufferVertexAttribCount);
	shaderManager->SetHitboxObjectBufferVertexAttribCount(hitboxObjectBufferVertexAttribCount);
	shaderManager->Init();
	shaderManager->SetVAO(vertices, verticesCount);
	shaderManager->SetHitboxVAO(hitboxVertices, Hitbox::indices, hitboxVerticesCount);
	shaderManager->SetHitboxComputeBuffers(hitboxVertices, hitboxVerticesCount);

	if (!LoadTexture(g_modelsDirectory + basicObject->textureName)) {
		WriteErrorToFile("Error loading texture!");
		WriteErrorToFile("Texture name: " + basicObject->textureName);
		throw std::exception();
	}
}

void StaticObject::Update() {
	//if (modelUpdate || viewUpdate || projectionUpdate) {
		//if (modelUpdate) 
		model = model * basicObject->globalTransform;

		glUseProgram(shaderManager->GetMainShader()->GetProgram());
		//if (modelUpdate) 
			glUniformMatrix4fv(shaderManager->GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));
		//if (viewUpdate) 
			glUniformMatrix4fv(shaderManager->GetViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
		//if (projectionUpdate)
			glUniformMatrix4fv(shaderManager->GetProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

		glUseProgram(shaderManager->GetHitboxShader()->GetProgram());
		//if (modelUpdate) 
			glUniformMatrix4fv(shaderManager->GetModelHitboxComputeLoc(), 1, GL_FALSE, glm::value_ptr(model));
		//if (viewUpdate) 
			glUniformMatrix4fv(shaderManager->GetViewHitboxLoc(), 1, GL_FALSE, glm::value_ptr(view));
		//if (projectionUpdate) 
			glUniformMatrix4fv(shaderManager->GetProjectionHitboxLoc(), 1, GL_FALSE, glm::value_ptr(projection));

		//modelUpdate = viewUpdate = projectionUpdate = false;
	//}

	UpdateHitboxes();
}

void StaticObject::Draw() {
	glUseProgram(shaderManager->GetMainShader()->GetProgram());
	glUniformMatrix4fv(shaderManager->GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(shaderManager->GetTextureAvailableLoc(), textureAvailable);

	if (textureAvailable) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shaderManager->GetTexture());
	}
	glBindVertexArray(shaderManager->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, verticesCount / objectBufferVertexAttribCount);

	glUseProgram(shaderManager->GetHitboxShader()->GetProgram());
	glUniformMatrix4fv(shaderManager->GetModelHitboxLoc(), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(shaderManager->GetHitboxVAO());
	glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 0);

	int i = 1;
	for (HitboxMap::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		int startVal = 8 * i;
		glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, startVal);
		++i;
	}
}

//-----------------------------------------------------------------------------
//----------------------------StaticObject::Private----------------------------
//-----------------------------------------------------------------------------

void StaticObject::SetVerticesBuffer() {
	verticesCount = basicObject->faces.size() * 3 * 5;
	vertices = new float[verticesCount];
	for (unsigned i = 0; i < basicObject->faces.size(); ++i) {
		for (unsigned j = 0; j < 3; ++j) {
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 0] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.x;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 1] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.y;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 2] = basicObject->controlPoints[basicObject->faces[i].indices[j]].coords.z;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 3] = basicObject->faces[i].uv[j].x;
			vertices[i * objectBufferVertexAttribCount * 3 + j * objectBufferVertexAttribCount + 4] = basicObject->faces[i].uv[j].y;
		}
	}
}

void StaticObject::UpdateHitboxes() {
	if (shaderManager->GetHitboxComputeShader() != nullptr) {
		glUseProgram(shaderManager->GetHitboxComputeShader()->GetProgram());
		glUniformMatrix4fv(shaderManager->GetModelHitboxComputeLoc(), 1, GL_FALSE, glm::value_ptr(model));

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderManager->GetHitboxComputeInBuffer());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shaderManager->GetHitboxComputeOutBuffer());

		glDispatchCompute(hitboxes.size(), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		void* ptr = nullptr;
		ptr = glMapNamedBuffer(shaderManager->GetHitboxComputeOutBuffer(), GL_READ_ONLY);
		if (ptr != nullptr) memcpy(hitboxOutVertices, ptr, hitboxVerticesCount * sizeof(float));
		glUnmapNamedBuffer(shaderManager->GetHitboxComputeOutBuffer());

		LoadHitboxDataFromShaders();
	}
}
