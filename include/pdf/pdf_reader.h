#pragma once

#include "pdf/pdf_file.h"
#include "pdf/pdf_tokens.h"
#include "resources/resource_manager.h"

#include <vector>
#include <string>

class PDFReader
{
public:
	~PDFReader();

	PDFFile* Read(const char* fp);
private:
	std::vector<PDFFile*> mPDFFiles;
	std::vector<Token> mTokens;
	void ProcessTokens();
};
