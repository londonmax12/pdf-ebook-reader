#pragma once

#include "pdf/pdf_tokens.h"
#include "resources/resource_manager.h"

#include <vector>
#include <string>
#include <unordered_map>

class PDFReader;

class PDFFile
{
public:
	PDFFile(const char* filepath) : mFilepath(filepath) {}
	std::string GetVersion() { return mVersion; }

	friend class PDFReader;
private:
	const char* mFilepath = "";

	std::string mVersion = "";
	size_t mXRefSize = 0;
	size_t mXRefStartObj = 0;
	std::unordered_map<int, XRefEntry> mXRefTable;
	size_t mTrailerSize = 0;
	XRefEntry mTrailerRoot{};
	XRefEntry mTrailerInfo{};
	std::string mFileID = "";
	std::string mUpdateID = "";

	ResourceManager mResources;
};