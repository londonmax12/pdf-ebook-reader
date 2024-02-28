#include "pdf/pdf_reader.h"

#include "pdf/pdf_tokenizer.h"

#include <fstream>
#include <iostream>
#include <string>

PDFReader::~PDFReader()
{
    for (auto& pdf : mPDFFiles) {
        delete pdf;
    }
}

PDFFile* PDFReader::Parse(const char* fp)
{
   
    PDFFile* pdf = new PDFFile();
    mPDFFiles.push_back(pdf);

    PDFTokenizer tokenizer{ fp };
    std::vector<Token> tokens = tokenizer.Tokenize();

    for (Token& token : tokens) {
        switch (token.Type)
        {
        case PDF_HEADER: {
            pdf->mVersion = token.Value.c_str();
            break;
        }
        case PDF_XREF_SIZE: {
            try {
                size_t size = std::stoi(token.Value);
                pdf->ResizeXRefTable(size);
            }
            catch (...) {
                std::cerr << "Invalid xRef table size\n";
                return nullptr;
            }
            break;
        }
        case PDF_XREF_START_OBJ: {
            try {
                size_t start = std::stoi(token.Value);
                pdf->mXRefStartObj = start;
            }
            catch (...) {
                std::cerr << "Invalid xRef table starting object\n";
                return nullptr;
            }
            break;
        }
        default:
            break;
        }
    }

    return pdf;
}
