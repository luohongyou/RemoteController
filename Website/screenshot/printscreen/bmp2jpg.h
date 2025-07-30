#pragma once

class CBmp2Jpeg
{
public:
	CBmp2Jpeg();
	~CBmp2Jpeg();

public:
	int Bmp2Jpeg(const char* bmp, const char* jpeg);

private:
	int SaveJpeg(const char* filename, unsigned char* bits, int width, int height, int depth);
	int ReadBmp(const char* bmp, unsigned char** data, int& w, int& h, int& d);
	void Bgra2Rgb(const unsigned char* src, int w, int h, int d, unsigned char* dst);
	void InitFileHeader(void* pFile, void* fileHeader);
	void InitInfoHeader(void* pFile, void* infoHeader);
	void SaveBmp(void* fileHeader, void* infoHeader, int bitCount, unsigned char* data, const char* savename);
private:
	int	m_quality;	//它的大小决定jpg的质量好坏

	enum {
		JPEG_QUALITY = 100,
	};
};
