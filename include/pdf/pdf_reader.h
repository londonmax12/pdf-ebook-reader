#pragma once
#include <vector>
#include <string>

class PDFFile
{
public:
	std::string mVersion;
};

class PDFReader
{
public:
	~PDFReader();

	PDFFile* Parse(const char* fp);

private:
	std::vector<PDFFile*> mPDFFiles;
};
