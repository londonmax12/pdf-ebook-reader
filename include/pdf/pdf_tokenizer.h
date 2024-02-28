#pragma once

#include <vector>
#include <string>

enum TokenType {
	PDF_UNKNOWN,
	PDF_HEADER,
	PDF_XREF_TABLE,
	PDF_XREF_START_OBJ,
	PDF_XREF_SIZE,
	PDF_XREF_ENTRY,
	PDF_TRAILER,
};

struct Token {
	Token(TokenType type, std::string value = "") : Type(type), Value(value) {}

	TokenType Type = PDF_UNKNOWN;
	std::string Value = "";
};

class PDFTokenizer {
public:
	PDFTokenizer(const char* fp);
	std::vector<Token> Tokenize();

	std::string PeekNextString();
	std::string GetNextString();

	std::string PeekNextLine();
	std::string GetNextLine();

private:
	const char* mFP = "";
	std::vector<char> mBuffer;
	int mCurrIndex = 0;
	std::string mCurrString = "";
};