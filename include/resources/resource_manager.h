#pragma once

#include <string>
#include <map>
#include <vector>

enum ResourceType
{
	Null,
	Bool,
	Interger,
	Real,
	Name,
	String,
	Array,
	Dictionary,
	Stream
};

struct Resource
{
	ResourceType Type;
	std::string Raw;
	union
	{
		bool BoolValue;
		int IntValue;
		double DoubleValue;
	};
	std::string NameValue;
	std::string StringValue;
	std::vector<Resource> ArrayValue;
	std::map<std::string, Resource> DictValue;
	std::vector<char> StreamValue;
};

class ResourceManager {
public:
	~ResourceManager();
	Resource* AllocateResource(int id, int genNumber);

private:
	std::map<std::string, Resource*> mResources;
};