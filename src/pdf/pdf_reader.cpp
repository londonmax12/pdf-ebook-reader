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
            pdf->mVersion = token.Value;
            break;
        }
        default:
            break;
        }
    }

    return pdf;
}
