#include "pdf/pdf_file.h"

void XRefTable::Resize(size_t size)
{
	mTable.resize(size);
}

void XRefTable::AddEntry(XRefEntry entry)
{
	int index = Hash(entry.ObjNumber);
	mTable[index] = entry;
}

XRefEntry XRefTable::GetEntry(size_t entry)
{
    int index = Hash(entry);
    return mTable[index];
}

int XRefTable::Hash(int key)
{
	return key % mTable.size();
}

void PDFFile::ResizeXRefTable(size_t size)
{
	mXRefSize = size;
	mXRefTable.Resize(size);
}
