#include <iostream>
#include <fstream>
#include <time.h>

// At the moment we only process bitmap (.bmp) files
// Resource used for creating bmp file:
// https://ricardolovelace.com/blog/creating-bitmap-images-with-c-on-windows/
#include "ImageHandler.h"

static const char FILENAME[] = "image.bmp";

int main(int argc, char** argv)
{
	int mode;
	std::cout << "	Type 1 or 2:\n \
---------------------------------------\n \
	1: Generate image.bmp\n \
	2: Read Data from image.bmp\n \
	3: Add Data to image.bmp\n \
---------------------------------------\n";

	std::cin >> mode;

	while (mode != 1 && mode != 2)
	{
		std::cout << "	Type 1 or 2:\n \
---------------------------------------\n \
	1: Generate image.bmp\n \
	2: Read Data from image.bmp\n \
	3: Add Data to image.bmp\n \
---------------------------------------\n";
		std::cin >> mode;
	}

	if (mode == 1)
	{
		int width = 400;
		int height = 400;
		int dpi = 96;

		// Should use singleton class instead
		Bitmap image(FILENAME, width, height, dpi);
		image.DrawImage();
		image.SaveImage();
	}
	else if (mode == 2)
	{
		std::ifstream fileExists(FILENAME);

		if (!fileExists.good())
			return 0;

		FILE* test2;
		fopen_s(&test2, "test2.txt", "wb");

		uint32_t dataSize = 0;
		uint8_t* pixelData = Bitmap::ReadImage("image.bmp", dataSize);
		for (uint32_t i = 0; i < dataSize; i++)
			fprintf(test2, "%u\n", pixelData[i]);
		
		fclose(test2);

		// Alter data
		system("pause");

		// Save the image
	}
}