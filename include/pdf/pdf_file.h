#pragma once

#include <vector>
#include <string>

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

class XRefTable
{
public:
	void Resize(size_t size);

	void AddEntry(XRefEntry entry);
	XRefEntry GetEntry(size_t entry);
private:
	int Hash(int key);
	std::vector<XRefEntry> mTable;
};

class PDFFile
{
public:
	std::string GetVersion() { return mVersion; }

	friend class PDFReader;
private:
	void ResizeXRefTable(size_t size);

	std::string mVersion = "";
	size_t mXRefSize = 0;
	size_t mXRefStartObj = 0;
	XRefTable mXRefTable;
};