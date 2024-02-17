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
static const char BITMAP_FILE_OUT[] = "image_steganogaphic.bmp";

static uint16_t WIDTH = 1024;
static uint16_t HEIGHT = 1024;
static uint16_t DPI = 96;

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
	// Should use singleton class instead
	Bitmap image(BITMAP_FILE, WIDTH, HEIGHT, DPI);
	image.DrawRandomImage();
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
	uint32_t dataSize = 0;
	uint8_t* pixelData = Bitmap::ReadImage(BITMAP_FILE, dataSize);

	// Check for size of embedding data
	FILE* embedFile;
	fopen_s(&embedFile, EMBED_SOURCE, "rb");
	assert(embedFile);
	fseek(embedFile, 0, SEEK_END);
	fseek(embedFile, 0, SEEK_SET);
	uint32_t embedSize = ftell(embedFile);
	uint8_t* embedData = new uint8_t[embedSize];
	fread(embedData, 1, embedSize, embedFile);

	if (embedSize > static_cast<uint32_t>(dataSize / 8) * 2)
		std::cout << "Your embed data can not be fit into the bitmap\n \
So data loss will occur\n";

	uint32_t colorIndex = 0;

	// Generate modified bitmap
	for (uint32_t i = 0; i < embedSize; i++)
	{
		// First character from embedData is read
		uint8_t bitChunk1 = embedData[i];
		uint8_t bitChunk2 = embedData[i];
		uint8_t bitChunk3 = embedData[i];
		uint8_t bitChunk4 = embedData[i];

		// Store necessary bits in only last 2 bits
		bitChunk1 = (bitChunk1 & 0b11000000) >> 6;
		bitChunk2 = (bitChunk2 & 0b00110000) >> 4;
		bitChunk3 = (bitChunk3 & 0b00001100) >> 2;
		bitChunk4 = (bitChunk4 & 0b00000011);

		// Store these bits inside a pixel array
		pixelData[colorIndex] = pixelData[colorIndex] & bitChunk1;
		colorIndex++;
		pixelData[colorIndex] = pixelData[colorIndex] & bitChunk2;
		colorIndex++;
		pixelData[colorIndex] = pixelData[colorIndex] & bitChunk3;
		colorIndex++;
		pixelData[colorIndex] = pixelData[colorIndex] & bitChunk4;
		colorIndex++;
	}

	// Save Bitmap
	Bitmap imageOut(BITMAP_FILE_OUT, WIDTH, HEIGHT, DPI);
	imageOut.AssignPixelData(pixelData);
	imageOut.SaveImage();

	// Free resources
	free(pixelData);
	fclose(embedFile);
}