#pragma once

#include "pdf/pdf_file.h"

#include <vector>
#include <string>

class PDFReader
{
public:
	~PDFReader();

	PDFFile* Parse(const char* fp);

	
private:
	std::vector<PDFFile*> mPDFFiles;
};
