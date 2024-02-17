#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>

// At the moment we only process bitmap (.bmp) files
// Resource used for creating bmp file:
// https://ricardolovelace.com/blog/creating-bitmap-images-with-c-on-windows/
#include "ImageHandler.h"

static const char BITMAP_FILE[] = "resources/image.bmp";
static const char EMBED_SOURCE[] = "resources/embed_source.txt";
static const char EMBED_EXTRACTED[] = "resources/embed_extracted.txt";
static const char BITMAP_FILE_OUT[] = "resources/image_steganogaphic.bmp";

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
	std::ifstream fileExists(BITMAP_FILE_OUT);

	if (!fileExists.good())
		return;

	// Pixels extracted and put into a file
	FILE* test2;
	fopen_s(&test2, EMBED_EXTRACTED, "w");
	assert(test2);
	uint32_t dataSize = 0;
	unsigned char* pixelData = Bitmap::ReadImage(BITMAP_FILE_OUT, dataSize);

	std::cout << "Please specify the amount of data to extract\n \
This should have been provided by the tool during embedding of data\n";
	uint32_t extractionSize = 0;
	std::cin >> extractionSize;
	if (extractionSize > dataSize)
	{
		std::cout << "The bitmap can not store that much data so it will all possible data\n";
		extractionSize = dataSize;
	}

	// To only get a number divisble by 4 since 1 byte = 8 bit and 2 bits are used for storage
	extractionSize = (extractionSize / 4) * 4;

	uint32_t colorIndex = 0;
	// Process data and add it to separate txt file
	for (uint32_t i = 0; i < extractionSize; i++)
	{
		// First character from embedData is read
		unsigned char bitChunk1 = pixelData[colorIndex];
		colorIndex++;
		unsigned char bitChunk2 = pixelData[colorIndex];
		colorIndex++;
		unsigned char bitChunk3 = pixelData[colorIndex];
		colorIndex++;
		unsigned char bitChunk4 = pixelData[colorIndex];
		colorIndex++;

		// Store necessary bits in only last 2 bits
		bitChunk1 = (bitChunk1 & 0b00000011) << 6;
		bitChunk2 = (bitChunk2 & 0b00000011) << 4;
		bitChunk3 = (bitChunk3 & 0b00000011) << 2;
		bitChunk4 = (bitChunk4 & 0b00000011);

		// Combine these bits now
		unsigned char character = (((bitChunk1 | bitChunk2) | bitChunk3) | bitChunk4);

		fprintf(test2, "%c", character);
	}

	fclose(test2);
}

static void EmbedMode()
{
	std::ifstream fileExists(BITMAP_FILE);

	if (!fileExists.good())
		return;

	// Read bitmap
	uint32_t dataSize = 0;
	unsigned char* pixelData = Bitmap::ReadImage(BITMAP_FILE, dataSize);

	// Check for size of embedding data
	FILE* embedFile;
	fopen_s(&embedFile, EMBED_SOURCE, "rb");
	assert(embedFile);
	fseek(embedFile, 0, SEEK_END);
	uint32_t embedSize = ftell(embedFile);
	fseek(embedFile, 0, SEEK_SET);
	unsigned char* embedData = new unsigned char[embedSize];
	fread(embedData, 1, embedSize, embedFile);

	if (embedSize > (dataSize / 8) * 2)
		std::cout << "Your embed data can not be fit into the bitmap\n \
So data loss will occur\n";

	uint32_t colorIndex = 0;

	// Generate modified bitmap
	for (uint32_t i = 0; i < embedSize; i++)
	{
		// First character from embedData is read
		unsigned char bitChunk1 = embedData[i];
		unsigned char bitChunk2 = embedData[i];
		unsigned char bitChunk3 = embedData[i];
		unsigned char bitChunk4 = embedData[i];

		// Store necessary bits in only last 2 bits
		bitChunk1 = (bitChunk1 & 0b11000000) >> 6;
		bitChunk2 = (bitChunk2 & 0b00110000) >> 4;
		bitChunk3 = (bitChunk3 & 0b00001100) >> 2;
		bitChunk4 = (bitChunk4 & 0b00000011);

		// Store these bits inside a pixel array
		pixelData[colorIndex] = (pixelData[colorIndex] & 0b11111100) | bitChunk1;
		colorIndex++;
		pixelData[colorIndex] = (pixelData[colorIndex] & 0b11111100) | bitChunk2;
		colorIndex++;
		pixelData[colorIndex] = (pixelData[colorIndex] & 0b11111100) | bitChunk3;
		colorIndex++;
		pixelData[colorIndex] = (pixelData[colorIndex] & 0b11111100) | bitChunk4;
		colorIndex++;

		/*
		// For debug purpose only
		// Store necessary bits in only last 2 bits
		bitChunk1 = (bitChunk1 & 0b00000011) << 6;
		bitChunk2 = (bitChunk2 & 0b00000011) << 4;
		bitChunk3 = (bitChunk3 & 0b00000011) << 2;
		bitChunk4 = (bitChunk4 & 0b00000011);

		// Combine these bits now
		unsigned char character = (((bitChunk1 | bitChunk2) | bitChunk3) | bitChunk4);
		*/
	}

	// Save Bitmap
	Bitmap imageOut(BITMAP_FILE_OUT, WIDTH, HEIGHT, DPI);
	imageOut.AssignPixelData(pixelData);
	imageOut.SaveImage();

	// Free resources
	free(pixelData);
	fclose(embedFile);

	std::cout << "Embed Data Size: " << embedSize << "\n";
	system("pause");
}