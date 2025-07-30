
#include <iostream>
#include <Windows.h>
#include <fstream>
#include "Bmp2Jpg.h"
#include <vector>

extern "C"
{
#include "jpeglib.h"
};

#pragma comment(lib, "libjpeg.lib")

using namespace std;

#pragma pack(2)  

struct bmp_fileheader   //文件头，长度为14Byte固定  
{
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
};

struct bmp_infoheader  //文件信息头，长度为40Byte固定  
{
	unsigned long biSize;
	unsigned long biWidth;
	unsigned long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	unsigned long biXPelsPerMeter;
	unsigned long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
};

struct RGBPallete
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char alpha;
};

CBmp2Jpeg::CBmp2Jpeg() :
	m_quality(JPEG_QUALITY)
{

}

CBmp2Jpeg::~CBmp2Jpeg()
{

}
/*===================================================================================
function:       jpeg压缩
input:          1:生成的文件名,2:bmp的指针,3:位图宽度,4:位图高度,5:颜色深度
return:         int
description:    bmp的像素格式为(RGB)
===================================================================================*/
int CBmp2Jpeg::SaveJpeg(const char* filename, unsigned char* bits, int width, int height, int depth)
{
	FILE* outfile;                 /* target file */
	fopen_s(&outfile, filename, "wb");
	if (outfile == NULL) {
		return -1;
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;      /* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3;         /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB;         /* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, m_quality, TRUE /* limit to baseline-JPEG values */);

	jpeg_start_compress(&cinfo, TRUE);

	JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
	int     row_stride;             /* physical row width in image buffer */
	row_stride = width * depth; /* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		//这里我做过修改，由于jpg文件的图像是倒的，所以改了一下读的顺序
		//row_pointer[0] = & bits[cinfo.next_scanline * row_stride];
		row_pointer[0] = &bits[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	jpeg_destroy_compress(&cinfo);
	return 0;
}

void CBmp2Jpeg::InitFileHeader(void* pFile, void* fileHeader)
{
	bmp_fileheader* pfileHeader = (bmp_fileheader*)fileHeader;
	fstream* filein = (fstream*)pFile;
	//初始化文件头
	pfileHeader->bfType = 0;
	pfileHeader->bfSize = 0;
	pfileHeader->bfReserved1 = 0;
	pfileHeader->bfReserved2 = 0;
	pfileHeader->bfOffBits = 0;
	//读位图文件头并输出相应信息
	filein->read((char*)fileHeader, sizeof(bmp_fileheader));
}

void CBmp2Jpeg::InitInfoHeader(void* pFile, void* infoHeader)
{
	bmp_infoheader* pinfoHeader = (bmp_infoheader*)infoHeader;
	fstream* filein = (fstream*)pFile;
	//初始化信息头
	pinfoHeader->biSize = 0;
	pinfoHeader->biWidth = 0;
	pinfoHeader->biHeight = 0;
	pinfoHeader->biPlanes = 0;
	pinfoHeader->biBitCount = 0;
	pinfoHeader->biCompression = 0;
	pinfoHeader->biSizeImage = 0;
	pinfoHeader->biXPelsPerMeter = 0;
	pinfoHeader->biYPelsPerMeter = 0;
	pinfoHeader->biClrUsed = 0;
	pinfoHeader->biClrImportant = 0;

	//读位图信息头并输出相应信息
	filein->read((char*)infoHeader, sizeof(bmp_infoheader));
}

void CBmp2Jpeg::SaveBmp(void* fileHeader, void* infoHeader, int bitCount, unsigned char* data, const char* savename)
{
	bmp_fileheader* pfileHeader = (bmp_fileheader*)fileHeader;
	bmp_infoheader* pinfoHeader = (bmp_infoheader*)infoHeader;
	//写入文件
	std::string str(savename);
	str.append(".bmp");
	fstream fileout;
	fileout.open(str, std::ios::binary | std::ios::out);
	fileout.write((char*)fileHeader, sizeof(bmp_fileheader));
	fileout.write((char*)infoHeader, sizeof(bmp_infoheader));
	fileout.write((char*)data, sizeof(unsigned char) * pfileHeader->bfSize - pfileHeader->bfOffBits);

	fileout.close();
}

//读取并将图片另存为一个新文件, 转换成rgb格式
int CBmp2Jpeg::ReadBmp(const char* bmp, unsigned char** data, int& w, int& h, int& d)
{
	//打开位图文件
	fstream filein;
	filein.open(bmp, std::ios::binary | std::ios::in);
	if (!filein.is_open())
	{
		char clog[256] = { 0 };
		sprintf_s(clog, sizeof(clog), "bmp转jpeg，找不到 %s\n", bmp);
		OutputDebugStringA(clog);
		return -1;
	}

	//定义变量
	long width = 0;
	long height = 0;
	long bitCount = 0;

	bmp_fileheader  fileHeader;
	bmp_infoheader  infoHeader;

	InitFileHeader(&filein, &fileHeader);

	if (fileHeader.bfType != 0x4d42)
	{
		filein.close();
		return -1;
	}

	InitInfoHeader(&filein, &infoHeader);

	width = infoHeader.biWidth;
	height = infoHeader.biHeight;
	bitCount = infoHeader.biBitCount;

	int bitPerLine = ((width * bitCount + 31) >> 5) << 2;
	int imgSize = abs(height * bitPerLine);
	int imgReal = fileHeader.bfSize - fileHeader.bfOffBits;
	if (imgSize != imgReal)
	{
		char clog[256] = { 0 };
		sprintf_s(clog, sizeof(clog), "bmp转jpeg,图像尺寸不对\n");
		OutputDebugStringA(clog);
		filein.close();
		return -1;
	}

	if (bitCount == 8)
	{
		std::vector<RGBPallete> palletes;
		unsigned char buf[256 * sizeof(RGBPallete)];

		filein.read((char*)buf, 256 * sizeof(RGBPallete));

		for (int i = 0; i < 256; i++)
		{
			RGBPallete pallete;
			memcpy(&pallete, buf + i * sizeof(RGBPallete), sizeof(RGBPallete));

			palletes.push_back(pallete);
		}

		unsigned char* pTemp = new unsigned char[imgSize];
		filein.read((char*)pTemp, imgSize);

		*data = new unsigned char[width * abs(height) * 4];
		for (int i = 0; i < imgSize; i++)
		{
			RGBPallete& p = palletes[pTemp[i]];
			memcpy((*data) + i * sizeof(RGBPallete), &p, sizeof(RGBPallete));
		}

		bitCount = 32;
		delete pTemp;
	}
	else if (bitCount == 24 || bitCount == 32)
	{
		*data = new unsigned char[imgSize];
		filein.read((char*)(*data), imgSize);
		filein.close();
	}
	else
	{
		filein.close();
		return -1;
	}

	w = width;
	h = height;
	d = bitCount;

	filein.close();

	return 0;
}

void CBmp2Jpeg::Bgra2Rgb(const unsigned char* src, int w, int h, int d, unsigned char* dst)
{
	unsigned char* pTempDst = dst;
	for (int i = 0; i < abs(h); i++)
	{
		const unsigned char* pTempSrc = nullptr;
		if (h > 0)
		{
			pTempSrc = src + w * i * d;
		}
		else
		{
			pTempSrc = src + w * abs(i + h + 1) * d;
		}

		for (int j = 0; j < w; j++)
		{
			*(pTempDst) = *(pTempSrc + 2);
			*(pTempDst + 1) = *(pTempSrc + 1);
			*(pTempDst + 2) = *(pTempSrc);
			pTempDst += 3;
			pTempSrc += d;
		}
	}
}


int CBmp2Jpeg::Bmp2Jpeg(const char* bmp, const char* jpeg)
{
	unsigned char* brga = nullptr; //指向位图buffer的全局指针，window下像素格式: BGRA(4个字节)
	int width = 0, height = 0, depth = 0;

	if (ReadBmp(bmp, &brga, width, height, depth) < 0)
	{
		return -1;
	}

	unsigned char* rgb = new unsigned char[width * abs(height) * depth / 8];
	Bgra2Rgb(brga, width, height, depth / 8, rgb);

	int ret = SaveJpeg(jpeg, rgb, width, abs(height), 3);

	delete[] brga;
	delete[] rgb;
	brga = nullptr;
	rgb = nullptr;
	return ret;
}