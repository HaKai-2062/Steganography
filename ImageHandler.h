#pragma once

struct rgbData
{
    uint8_t r, g, b;
};

class Bitmap
{
public:
	Bitmap() = delete;
	Bitmap& operator=(Bitmap& other) = delete;
	Bitmap(Bitmap& other) = delete;

	Bitmap(const char* fileName, int width, int height, int dpi)
		: m_FileName(fileName), m_Width(width), m_Height(height), m_Dpi(dpi) {}
	
	void AssignPixelData(uint8_t* pixelData);
	void DrawRandomImage();
	void SaveImage();
    static unsigned char* ReadImage(const char* filename, size_t& dataSize);

private:
	int m_Width, m_Height;
	int m_Dpi;
	const char* m_FileName;
	rgbData* m_PixelData;
};
