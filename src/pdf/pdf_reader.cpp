#include "pdf/pdf_reader.h"

#include "pdf/pdf_tokenizer.h"
#include "pdf/pdf_parser.h"

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

PDFFile* PDFReader::Read(const char* fp)
{
    PDFFile* pdf = new PDFFile(fp);
    mPDFFiles.push_back(pdf);

    PDFTokenizer tokenizer{ pdf->mFilepath };
    if (!tokenizer.Tokenize(mTokens))
        return nullptr;

    PDFParser parser{ mTokens, &pdf->mResources };
    if (!parser.Parse())
        return nullptr;

    ProcessTokens();
    
    return pdf;
}

void PDFReader::ProcessTokens()
{
    PDFFile* pdf = mPDFFiles.back();

    for (int i = 0; i < mTokens.size(); i++) {
        auto& token = mTokens[i];

        switch (token.Type)
        {
        case PDF_VERSION:
            pdf->mVersion = std::get<std::string>(token.Value);
            break;
        case PDF_XREF:
            if ((i + 2) >= mTokens.size())
                return;

            i++;
            pdf->mXRefStartObj = std::get<size_t>(mTokens[i].Value);
            i++;
            pdf->mXRefSize = std::get<size_t>(mTokens[i].Value);
            for (int j = 0; j < pdf->mXRefSize; j++) {
                i++;
                XRefEntry t = std::get<XRefEntry>(mTokens[i].Value);
                
                pdf->mXRefTable[t.Object] = t;
            }
            break;
        case PDF_TRAILER:
            if ((i + 5) >= mTokens.size())
                return;
            for (int j = 0; j < 5; j++) {
                i++;
                if (mTokens[i].Type != PDF_VAR) return;
                
                Var& var = std::get<Var>(mTokens[i].Value);
                if (var.Name == "Size") {
                    pdf->mTrailerSize = std::get<size_t>(var.Value);
                }
                else if (var.Name == "Root") {
                    pdf->mTrailerRoot = std::get<XRefEntry>(var.Value);
                }
                else if (var.Name == "Info") {
                    pdf->mTrailerInfo = std::get<XRefEntry>(var.Value);
                }
                else if (var.Name == "FileID") {
                    pdf->mFileID = std::get<std::string>(var.Value);
                }
                else if (var.Name == "UpdateID") {
                    pdf->mUpdateID = std::get<std::string>(var.Value);
                }
            }
            break;
        default:
            break;
        }
    }
}
