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
            continue;
        }
        if (mCurrString == "xref") {
            tokens.push_back({ TokenType::PDF_XREF_TABLE });
            std::string details = GetNextLine();
            size_t space_pos = details.find(' ');
            if (space_pos == std::string::npos) {
                std::cerr << "Invalid xref table details";
                return tokens;
            }

            tokens.push_back({ TokenType::PDF_XREF_START_OBJ, details.substr(0, space_pos) });
            tokens.push_back({ TokenType::PDF_XREF_SIZE, details.substr(space_pos + 1) });

            std::string nextLine = GetNextLine();
            while (nextLine != "trailer")
            {
                tokens.push_back({ TokenType::PDF_XREF_ENTRY, nextLine });
                nextLine = GetNextLine();
            }
            tokens.push_back({ TokenType::PDF_TRAILER, ""});           
            GetNextLine();
            std::string str = GetNextString();
            while (str != ">>")
            {
                if (str == "/Size") {
                    tokens.push_back({ TokenType::PDF_TRAILER_SIZE, GetNextString() });
                }
                if (str == "/Root") {
                    tokens.push_back({ TokenType::PDF_TRAILER_ROOT, GetNextString(true) });
                }
                if (str == "/ID") {
                    std::string initialID = GetNextString(false);
                    initialID = initialID.substr(2, initialID.size() - 3);

                    std::string updateID = GetNextString(false);
                    updateID = updateID.substr(1, updateID.size() - 3);

                    tokens.push_back({ TokenType::PDF_TRAILER_INITIAL_ID, initialID });
                    tokens.push_back({ TokenType::PDF_TRAILER_UPDATE_ID, updateID });
                }

                if (str == "/Info") {
                    tokens.push_back({ TokenType::PDF_TRAILER_INFO, GetNextString(true) });
                }
                str = GetNextString();
            }
            std::string d = GetNextLine();
            std::string g = GetNextLine();
            PeekNextLine();
            continue;
        }
    }

    return tokens;
}

std::string PDFTokenizer::PeekNextString()
{
    std::string str = "";
    int i = mCurrIndex + 1;
    char c = mBuffer[i];

    if (isspace(c)) {
        i++;
        c = mBuffer[i];
    }

    while (c != '\0' && !isspace(c)) {
        str += c;
        i++;
        c = mBuffer[i];
    }
    return str;
}

std::string PDFTokenizer::GetNextString(bool incSpace)
{
    std::string str = "";
    char c = mBuffer[mCurrIndex];

    if (isspace(c)) {
        mCurrIndex++;
        c = mBuffer[mCurrIndex];
    }

    while (c != '\0') {
        if ((isspace(c) && !incSpace) || c == '\n')
            break;
        str += c;
        mCurrIndex++;
        c = mBuffer[mCurrIndex];
    }
    mCurrString = "";
    return str;
}

std::string PDFTokenizer::PeekNextLine()
{
    std::string str = "";

    char c = mBuffer[mCurrIndex];
    int i = mCurrIndex;

    while (c != '\0' && c != '\n') {
        i++;
        c = mBuffer[i];
    }

    for (int j = mCurrIndex; j <= i; j++) {
        str += mBuffer[j];
    }

    return str;
}

std::string PDFTokenizer::GetNextLine()
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
    mCurrString = "";
    return str;
}
