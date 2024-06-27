#pragma once

#include <stdint.h>

struct rgbData
{
    uint8_t r, g, b;
};

class Bitmap
{
public:
	Bitmap(const char* fileName, int width, int height, int dpi)
		: m_FileName(fileName), m_Width(width), m_Height(height), m_Dpi(dpi) {}
    ~Bitmap();
	
	void AssignPixelData(uint8_t* pixelData);
	void DrawRandomImage();
	void SaveImage();
    static unsigned char* ReadImage(const char* filename, uint32_t& width, uint32_t& height, uint32_t& dpi);

private:
	Bitmap() = delete;
	Bitmap& operator=(Bitmap& other) = delete;
	Bitmap(Bitmap& other) = delete;

private:
	int m_Width, m_Height;
	int m_Dpi;
	const char* m_FileName;
	rgbData* m_PixelData;
};
