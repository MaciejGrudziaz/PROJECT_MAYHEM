#pragma once
#include <vector>

template <class T>
class AutoList {
	static std::vector<T*> objects;

public:
	AutoList() {
		objects.push_back(static_cast<T*>(this));
	}

	static int GetCount() { return objects.size(); }
	static T* GetObj(int idx) {
		if (idx >= 0 && idx < objects.size())
			return objects[idx];
		else return nullptr;
	}
	static bool DestoryObj(int idx) {
		if (idx >= 0 && idx < objects.size()) {
			objects.erase(objects.begin() + idx);
			return true;
		}
		else return false;
	}
	static void ClearAll() {
		T* obj;
		while (objects.size() > 0) {
			obj = objects.back();
			objects.pop_back();
			delete obj;
		}
	}
	static bool ChangePos(int idx1, int idx2) {
		if(idx1>=0 && idx1<objects.size())
			if(idx2>=0 && idx2<objects.size())
				if (idx1 != idx2) {
					T* tmp = objects[idx1];
					objects[idx1] = objects[idx2];
					objects[idx2] = tmp;
					return true;
				}
		
		return false;
	}

	virtual ~AutoList() {
		T* thisObj = static_cast<T*>(this);
		for (std::vector<T*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if (*it == thisObj) {
				objects.erase(it);
				break;
			}
		}
	}
};

template <class T>
std::vector<T*> AutoList<T>::objects = std::vector<T*>();
