#pragma once
#include <vector>
#include "BasicObject.h"
#include "Shader.h"
#include "AutoList.h"

struct BasicModel:public AutoList<BasicModel>{
	std::vector<BasicObject*> objects;

	BasicModel() { }

	void LoadObject(BasicObject* object_) { if (object_ != nullptr) objects.push_back(object_); }

	~BasicModel() {
		for (BasicObject* obj : objects)
			delete obj;
	}
};