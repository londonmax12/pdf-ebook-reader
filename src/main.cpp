#include <stdio.h>
#include "pdf/pdf_reader.h"

int main() {
	PDFReader reader{};
	PDFFile* res = reader.Read("examples/pride_and_prejudice.pdf");

	if (!res) {
		printf("Failed to read PDF");
		return 1;
	}
}