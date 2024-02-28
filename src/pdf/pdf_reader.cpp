#include "pdf/pdf_reader.h"

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
    std::ifstream file(fp, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return nullptr;
    }

    PDFFile* pdf = new PDFFile();
    mPDFFiles.push_back(pdf);

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() >= 5 && line.substr(0, 5) == "%PDF-") {
            pdf->mVersion = line.substr(5);
            break;
        }
    }

    file.close();

    return pdf;
}
