#pragma once

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <type_traits>

enum ResourceType
{
	TYPE_NONE,
	TYPE_BOOL,
	TYPE_INTEGER,
	TYPE_REAL,
	TYPE_NAME,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_DICT,
	TYPE_STREAM
};

enum StreamFilter {
	FILTER_UNKNOWN,
	FILTER_FLATEDECODE
};

struct Stream {
	size_t Size = 0;
	StreamFilter Filter = FILTER_UNKNOWN;
	char* Data = nullptr;

	Stream() = default;
	Stream(const size_t& size);

	void Allocate(const size_t& size);
};

struct Resource
{
	~Resource();

	ResourceType Type = TYPE_NONE;
	std::string Raw;
	union
	{
		bool BoolValue;
		int IntValue;
		double DoubleValue;
		Stream StreamValue{};
	};
	std::string NameValue;
	std::string StringValue;
	std::vector<Resource> ArrayValue;
	std::map<std::string, Resource> DictValue;
};

class ResourceManager {
public:
	~ResourceManager();

	template <typename T>
	Resource* AllocateResource(int id, int genNumber, T res) {
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

		if (std::is_same<T, Stream>::value) {
			resource->Type = TYPE_STREAM;
			resource->StreamValue = res;
		}
		else return nullptr;

		return resource;
	}

private:
	std::map<std::string, Resource*> mResources;
};