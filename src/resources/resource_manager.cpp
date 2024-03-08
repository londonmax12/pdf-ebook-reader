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

Resource::~Resource()
{
	if (Type == TYPE_STREAM)
		if (StreamValue.Size && StreamValue.Data)
			free(StreamValue.Data);
}

Stream::Stream(const size_t& size)
{
	Allocate(size);
}

void Stream::Allocate(const size_t& size)
{
	Size = size;
	Data = (char*)malloc(Size);
}
