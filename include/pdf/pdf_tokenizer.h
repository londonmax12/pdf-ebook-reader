#pragma once

#include "pdf/pdf_tokens.h"

#include <variant>
#include <vector>
#include <string>
#include <fstream>

class PDFTokenizer {
public:
	PDFTokenizer(const char* fp);
	bool Tokenize(std::vector<Token>& tokens);

private:
	void AddToken(std::vector<Token>& tokens, const std::string& tokenString, bool isNumber) const;
	const char* mFP = "";
	std::ifstream mFile;
	std::streampos mFileSize;
};