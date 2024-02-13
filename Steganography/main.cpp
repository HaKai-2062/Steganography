#include <iostream>
#include <time.h>

// At the moment we only process bitmap (.bmp) files
// Resource used for creating bmp file:
// https://ricardolovelace.com/blog/creating-bitmap-images-with-c-on-windows/
#include "ImageHandler.h"

int main(int argc, char** argv)
{
	int mode;
	std::cout << "	Type 1 or 2:\n \
---------------------------------------\n \
	1: Generate image.bmp\n \
	2: Read Data from image.bmp\n \
---------------------------------------\n";

	std::cin >> mode;

	while (mode != 1 && mode != 2)
	{
		std::cout << "	Type 1 or 2:\n \
---------------------------------------\n \
	1: Generate image.bmp\n \
	2: Read Data from image.bmp\n \
---------------------------------------\n";
		std::cin >> mode;
	}

	if (mode == 1)
	{
		int width = 400;
		int height = 400;
		int dpi = 96;

		// Should use singleton class instead
		Bitmap image("image.bmp", width, height, dpi);
		image.DrawImage();
		image.SaveImage();
	}
}