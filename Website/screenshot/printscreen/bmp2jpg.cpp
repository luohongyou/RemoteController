
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

struct bmp_fileheader   //�ļ�ͷ������Ϊ14Byte�̶�  
{
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
};

struct bmp_infoheader  //�ļ���Ϣͷ������Ϊ40Byte�̶�  
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
function:       jpegѹ��
input:          1:���ɵ��ļ���,2:bmp��ָ��,3:λͼ���,4:λͼ�߶�,5:��ɫ���
return:         int
description:    bmp�����ظ�ʽΪ(RGB)
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
		//�����������޸ģ�����jpg�ļ���ͼ���ǵ��ģ����Ը���һ�¶���˳��
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
	//��ʼ���ļ�ͷ
	pfileHeader->bfType = 0;
	pfileHeader->bfSize = 0;
	pfileHeader->bfReserved1 = 0;
	pfileHeader->bfReserved2 = 0;
	pfileHeader->bfOffBits = 0;
	//��λͼ�ļ�ͷ�������Ӧ��Ϣ
	filein->read((char*)fileHeader, sizeof(bmp_fileheader));
}

void CBmp2Jpeg::InitInfoHeader(void* pFile, void* infoHeader)
{
	bmp_infoheader* pinfoHeader = (bmp_infoheader*)infoHeader;
	fstream* filein = (fstream*)pFile;
	//��ʼ����Ϣͷ
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

	//��λͼ��Ϣͷ�������Ӧ��Ϣ
	filein->read((char*)infoHeader, sizeof(bmp_infoheader));
}

void CBmp2Jpeg::SaveBmp(void* fileHeader, void* infoHeader, int bitCount, unsigned char* data, const char* savename)
{
	bmp_fileheader* pfileHeader = (bmp_fileheader*)fileHeader;
	bmp_infoheader* pinfoHeader = (bmp_infoheader*)infoHeader;
	//д���ļ�
	std::string str(savename);
	str.append(".bmp");
	fstream fileout;
	fileout.open(str, std::ios::binary | std::ios::out);
	fileout.write((char*)fileHeader, sizeof(bmp_fileheader));
	fileout.write((char*)infoHeader, sizeof(bmp_infoheader));
	fileout.write((char*)data, sizeof(unsigned char) * pfileHeader->bfSize - pfileHeader->bfOffBits);

	fileout.close();
}

//��ȡ����ͼƬ���Ϊһ�����ļ�, ת����rgb��ʽ
int CBmp2Jpeg::ReadBmp(const char* bmp, unsigned char** data, int& w, int& h, int& d)
{
	//��λͼ�ļ�
	fstream filein;
	filein.open(bmp, std::ios::binary | std::ios::in);
	if (!filein.is_open())
	{
		char clog[256] = { 0 };
		sprintf_s(clog, sizeof(clog), "bmpתjpeg���Ҳ��� %s\n", bmp);
		OutputDebugStringA(clog);
		return -1;
	}

	//�������
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
		sprintf_s(clog, sizeof(clog), "bmpתjpeg,ͼ��ߴ粻��\n");
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
	unsigned char* brga = nullptr; //ָ��λͼbuffer��ȫ��ָ�룬window�����ظ�ʽ: BGRA(4���ֽ�)
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