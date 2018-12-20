#pragma once
#include <map>
#include <string>
#include <vector>

template <class T>
class AutoMap {
	typedef std::map<std::string, T*> ObjMap;

	static ObjMap objects;

	std::string name;

public:
	AutoMap() {
		name = std::to_string(objects.size());
		objects.insert(ObjMap::value_type(name, static_cast<T*>(this)));
	}

	AutoMap(std::string name_) :name(name_) {
		objects.insert(ObjMap::value_type(name_, static_cast<T*>(this)));
	}

	static int GetCount() { return objects.size(); }

	static T* GetObj(std::string name_) {
		ObjMap::iterator it = objects.find(name_);
		if (it != objects.end())
			return it->second;
		else return nullptr;
	}

	std::string GetName() { return name; }

	static bool DestoryObj(std::string name_) {
		ObjMap::iterator it = objects.find(name_);
		if (it != objects.end()) {
			objects.erase(it);
			return true;
		}
		else return false;
	}

	static void ClearAll() {
		for (ObjMap::iterator it = objects.begin(); it != objects.end(); ++it)
			delete it->second;

		objects.clear();
	}

	virtual ~AutoMap() {
		T* thisObj = static_cast<T*>(this);
		for (ObjMap::iterator it = objects.begin(); it != objects.end(); ++it) {
			if (it->second == thisObj) {
				objects.erase(it);
				break;
			}
		}
	}

};

template<class T>
std::map<std::string, T*> AutoMap<T>::objects = std::map<std::string, T*>();
