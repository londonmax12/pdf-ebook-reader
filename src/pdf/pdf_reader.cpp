#include "pdf/pdf_reader.h"

#include "pdf/pdf_tokenizer.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

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
                pdf->mXRefSize = std::stoi(token.Value);
            }
            catch (...) {
                std::cerr << "Invalid xRef table size\n";
                return nullptr;
            }
            break;
        }
        case PDF_XREF_START_OBJ: {
            try {
                pdf->mXRefStartObj = std::stoi(token.Value);
            }
            catch (...) {
                std::cerr << "Invalid xRef table starting object\n";
                return nullptr;
            }
            break;
        }
        case PDF_XREF_ENTRY: {
            std::istringstream ss(token.Value);
            std::string obj, gen, status;
            if (ss >> obj >> gen >> status) {
                try {
                    int o = std::stoi(obj);
                    pdf->mTable[o] = { o, std::stoi(gen), status == "f" ? XRefFree : XRefInUse };
                }
                catch (...) {
                    std::cerr << "Invalid xRef table entry\n";
                }
            }
            else {
                std::cerr << "Invalid xRef table entry\n";
                return nullptr;
            }
        }
        default:
            break;
        }
    }

    return pdf;
}
