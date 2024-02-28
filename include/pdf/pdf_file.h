#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class PDFReader;

enum XRefStatus {
	XRefNone,
	XRefFree,
	XRefInUse
};

struct XRefEntry
{
	XRefEntry() = default;
	XRefEntry(int objNumber, int generationNumber, XRefStatus status)
		: ObjNumber(objNumber), GenerationNumber(generationNumber), Status(status) {};

	int ObjNumber = 0;
	int GenerationNumber = 0;
	XRefStatus Status = XRefNone;
};

class PDFFile
{
public:
	std::string GetVersion() { return mVersion; }

	friend class PDFReader;
private:
	std::string mVersion = "";
	size_t mXRefSize = 0;
	size_t mXRefStartObj = 0;
	std::unordered_map<int, XRefEntry> mTable;
};