#include "pdf/pdf_tokenizer.h"

#include <iostream>
#include <sstream>

PDFTokenizer::PDFTokenizer(const char* fp)
    : mFP(fp), mFile(fp, std::ios::binary)
{
    if (!mFile.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    mFile.seekg(0, std::ios::end);
    mFileSize = mFile.tellg();
    mFile.seekg(0, std::ios::beg);
}

bool PDFTokenizer::Tokenize(std::vector<Token>& tokens)
{
    tokens.clear();

    std::string currString;
    bool isNumber = true;

    while (!mFile.eof()) {
        char c = mFile.get();
        unsigned char uc = static_cast<unsigned char>(c);

        if (c == '\n') {
            tokens.push_back({ PDF_ENDL, "" });
            if (!currString.empty()) {
                AddToken(tokens, currString, isNumber);
                currString.clear();
                isNumber = true;
            }
        }
        else if (uc < 128 && isspace(c)) {
            if (!currString.empty()) {
                AddToken(tokens, currString, isNumber);
                currString.clear();
                isNumber = true;
            }
        }
        else {
            currString.push_back(c);
            if (uc >= 128 || !isdigit(c)) {
                isNumber = false;
            }
        }
    }

    if (!currString.empty()) {
        AddToken(tokens, currString, isNumber);
    }

    return true;
}

void PDFTokenizer::AddToken(std::vector<Token>& tokens, const std::string& tokenString, bool isNumber) const
{
    if (isNumber) {
        try {
            int number = std::stoi(tokenString);
            tokens.push_back({ PDF_INT, std::to_string(number) });
        }
        catch (const std::exception&) {
            throw std::runtime_error("Invalid number");
        }
        return;
    }
    if (!tokenString.empty() && tokenString[0] == '/') {
        tokens.push_back({ PDF_NAME, tokenString.substr(1) });
    }
    else if (tokenString.size() >= 6 && tokenString.substr(0, 5) == "%PDF-" && isdigit(tokenString[5])) {
        size_t start = tokenString.find("-") + 1;
        size_t end = tokenString.find(".", start);
        tokens.push_back({ PDF_VERSION, tokenString.substr(start, end + 1) });
    }

    else if (tokenString.size() == 1) tokens.push_back({ PDF_CHAR, tokenString });
    else if (tokenString == "xref") tokens.push_back({ PDF_XREF, "" });
    else if (tokenString == "trailer") tokens.push_back({ PDF_TRAILER, "" });
    else if (tokenString == "obj") tokens.push_back({ PDF_OBJ, "" });
    else if (tokenString == ">>") tokens.push_back({ PDF_END, "" });
    else if (tokenString == "<<") tokens.push_back({ PDF_BEGIN, "" });

    else tokens.push_back({ PDF_STRING, tokenString });
}
