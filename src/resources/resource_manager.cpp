#include "resources/resource_manager.h"

#include <sstream>

ResourceManager::~ResourceManager()
{
	for (auto i = mResources.begin(); i != mResources.end(); ++i) {
		if (i->second)
			delete i->second;
	}
	mResources.clear();
}

Resource* ResourceManager::AllocateResource(int id, int genNumber)
{
	std::ostringstream ss;
	ss << id << "_" << genNumber;
	std::string key = ss.str();
	if (mResources.count(key) != 0 && mResources[key]) {
		delete mResources[key];
		mResources[key] = nullptr;
	}

	Resource* resource = new Resource();
	resource->NameValue = key;
	mResources[key] = resource;

	return resource;
}
