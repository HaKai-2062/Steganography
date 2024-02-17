#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>

// At the moment we only process bitmap (.bmp) files
// Resource used for creating bmp file:
// https://ricardolovelace.com/blog/creating-bitmap-images-with-c-on-windows/
#include "ImageHandler.h"

static const char BITMAP_FILE[] = "image.bmp";
static const char EMBED_SOURCE[] = "embed_source.txt";
static const char EMBED_EXTRACTED[] = "embed_extracted.txt";
static const char BITMAP_FILE_OUT[] = "image_steganogaphic.txt";

static void GenerateMode();
static void ReadMode();
static void EmbedMode();

int main(int argc, char** argv)
{
	int mode;
	std::cout << "	Type 1, 2 or 3:\n \
---------------------------------------\n \
	1: Generate \""<< BITMAP_FILE <<"\" with random data\n \
	2: Read \"" << BITMAP_FILE << "\" data into \"" << EMBED_EXTRACTED << "\"\n \
	3: Embed \"" << EMBED_SOURCE << "\" into \"" << BITMAP_FILE << "\"\n \
---------------------------------------\n";

	std::cin >> mode;

	while (mode != 1 && mode != 2 && mode != 3)
	{
		std::cout << "	Type 1, 2 or 3:\n \
---------------------------------------\n \
	1: Generate \"" << BITMAP_FILE << "\" with random data\n \
	2: Read \"" << BITMAP_FILE << "\" data into \"" << EMBED_EXTRACTED << "\"\n \
	3: Embed \"" << EMBED_SOURCE << "\" into \"" << BITMAP_FILE << "\"\n \
---------------------------------------\n";
		std::cin >> mode;
	}

	switch (mode)
	{
	case 1:
		GenerateMode();
		break;
	case 2:
		ReadMode();
		break;
	case 3:
		EmbedMode();
		break;
	default:
		std::cout << "Bad input\n";
		break;
	}
}

static void GenerateMode()
{
	int width = 400;
	int height = 400;
	int dpi = 96;

	// Should use singleton class instead
	Bitmap image(BITMAP_FILE, width, height, dpi);
	image.DrawImage();
	image.SaveImage();
}

static void ReadMode()
{
	std::ifstream fileExists(BITMAP_FILE);

	if (!fileExists.good())
		return;

	// Pixels extracted into a file
	FILE* test2;
	fopen_s(&test2, EMBED_EXTRACTED, "wb");
	assert(test2);
	uint32_t dataSize = 0;
	uint8_t* pixelData = Bitmap::ReadImage(BITMAP_FILE, dataSize);
	for (uint32_t i = 0; i < dataSize; i++)
		fprintf(test2, "%u\n", pixelData[i]);

	fclose(test2);

	// TDL: Will ask for data length and return the data 
	// data = length where length < maxStorageSize in that image

	// Process data and add it to separate txt file
}

static void EmbedMode()
{
	std::ifstream fileExists(BITMAP_FILE);

	if (!fileExists.good())
		return;

	// Read bitmap
	// Check for size of bitmap data
	// Check for size of embedding data
	// Generate modified bitmap
}