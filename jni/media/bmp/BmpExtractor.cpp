/*
 * BmpParser.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#include "BmpExtractor.h"
#define TAG "Extractor"
#include "log.h"

namespace peng {

typedef unsigned char   BYTE;  /*һ���ֽڵ�����*/
typedef unsigned short int  WORD;  /*�����ֽڵ�����*/
typedef unsigned int   DWORD;  /*�ĸ��ֽڵ�����*/


/*�ļ�ͷ�ṹ*/
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;            /*�ļ����ͣ�BMΪBMP*/
    DWORD bfSize;           /*�ļ��Ĵ�С(�ֽ�)*/
    WORD bfReserved1;       /*����*/
    WORD bfReserved2;       /*����*/
    DWORD bfOffBits;        /*���ṹ��ʼ����ͼ�����е�ƫ���ֽ���*/
}BITMAPFILEHEADER;

/*�ļ���Ϣͷ�ṹ*/
typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;           /*���ṹ���ֽ���*/
  DWORD biWidth;          /*���������ʾͼ�εĿ��*/
  DWORD biHeight;         /*���������ʾͼ�εĸ߶�*/
  WORD  biPlanes;         /*ɫ��ƽ�������Ϊ1*/
  WORD  biBitCount;       /*һ��������ռ��λ�������1��4��8��24*/
  DWORD biCompression;    /*ͼ��ѹ����ʽ��0Ϊ��ѹ��*/
  DWORD biSizeImage;      /*ͼ�ε��ֽ���*/
  DWORD biXPelsperMeter;  /*1���ڵ�ˮƽ������*/
  DWORD biYPelsPerMeter;  /*1���ڵĴ�ֱ������*/
  DWORD biClrUsed;        /*ͼ����ʵ���õ���������*/
  DWORD biClrlmportant;   /*ͼ���е���Ҫ����ɫ��*/
}BITMAPINFOHEADER;

static int bitmapHeaderSize() {
    //return sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    return (1+2+1+1+1+2)*2 + (2+2+2+1+1+2+2+2+2+2+2)*2;
}

BmpExtractor::BmpExtractor(DataSource& source) : mSource(source) {
    // TODO Auto-generated constructor stub
    PRINTI("filesize: %llu", mSource.getSize());
    uint64_t contentLength = mSource.getSize() - bitmapHeaderSize();
    int w = 920;
    int h = 1248;
    int bitDepth = 24;
    uint64_t calcLen = w*h*3;
    PRINTI("cl:%llu, calc:%llu", contentLength, calcLen);
}

BmpExtractor::~BmpExtractor() {
    // TODO Auto-generated destructor stub
}

}

