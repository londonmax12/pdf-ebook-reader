#include "pdf/pdf_tokenizer.h"

#include <fstream>
#include <iostream>

PDFTokenizer::PDFTokenizer(const char* fp)
    : mFP(fp)
{
}

std::vector<Token> PDFTokenizer::Tokenize()
{
    std::vector<Token> tokens{};

    std::ifstream file(mFP, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return tokens;
    } 

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    (fileSize);

    mBuffer = std::vector<char>(fileSize);

    file.read(mBuffer.data(), fileSize);

    file.close();

    
    for (; mCurrIndex < mBuffer.size(); mCurrIndex++) {
        char c = mBuffer[mCurrIndex];

        if (c == '\n') {
            mCurrString = "";
            continue;
        }

        mCurrString += c;
        if (mCurrString == "%PDF-")
        {
            std::string str = GetNextString();

            tokens.push_back({ TokenType::PDF_HEADER, str });

            mCurrIndex += str.size();
            mCurrString = "";
            continue;
        }
        if (mCurrString == "xref") {
            tokens.push_back({ TokenType::PDF_XREF_TABLE });
            std::string details = SkipLine();
            size_t space_pos = details.find(' ');
            if (space_pos != std::string::npos) {
                tokens.push_back({ TokenType::PDF_XREF_START_OBJ, details.substr(0, space_pos) });
                tokens.push_back({ TokenType::PDF_XREF_SIZE, details.substr(space_pos + 1) });
            }

            continue;
        }
    }

    return tokens;
}

std::string PDFTokenizer::GetNextString()
{
    std::string str = "";
    int i = mCurrIndex + 1;
    char c = mBuffer[i];

    while (c != '\0') {
        if (isspace(c))
            break;

        str += c;
        i++;
        c = mBuffer[i];
    }
    return str;
}

std::string PDFTokenizer::SkipLine()
{
    std::string str = "";

    char c = mBuffer[mCurrIndex];

    while (c != '\0') {
        mCurrIndex++;
        if (c == '\n')
            break;
        c = mBuffer[mCurrIndex];
    }

    c = mBuffer[mCurrIndex];

    while (c != '\0') {
        if (c == '\n')
            break;

        str += c;
        mCurrIndex++;
        c = mBuffer[mCurrIndex];
    }
    return str;
}
