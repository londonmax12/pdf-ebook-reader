#include <stdio.h>
#include "pdf/pdf_reader.h"

int main() {
	PDFReader reader{};
	PDFFile* res = reader.Parse("examples/pride_and_prejudice.pdf");

	if (!res) {
		printf("Failed to parse PDF");
		return 1;
	}
}