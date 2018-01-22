/*
 * Bitmap.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: richie
 */

#include "Bitmap.h"
#include <string.h>

#define TAG "Bitmap"
#include "log.h"

void Bitmap::save2File(unsigned char* data, int w, int h, int bitdepth) {
    DWORD BytePerLine,FillZeroNum,PixelN;
    char Zeros[5]="\0\0\0\0";
    const char *BMtype="BM";
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;

    FILE* fpout = fopen("temp.bmp", "wb");
    BytePerLine = (bitdepth*w + bitdepth)&0xFFFFFFFC;
    FillZeroNum = (bitdepth*w)&0x03;

    fileheader.bfType = *((WORD *)(BMtype));
    fileheader.bfSize = BITMAP_HEADER_LEN + h*BytePerLine;
    fileheader.bfReserved1 = 0;
    fileheader.bfReserved2 = 0;
    fileheader.bfOffBits = BITMAP_HEADER_LEN;
    infoheader.biSize = 40;
    infoheader.biWidth=w;
    infoheader.biHeight=h;
    infoheader.biPlanes=1;
    infoheader.biBitCount=bitdepth*8;
    infoheader.biCompression=0;
    infoheader.biSizeImage=h*BytePerLine;
    infoheader.biXPelsperMeter=(WORD)(300*39.37007874);
    infoheader.biYPelsPerMeter=(WORD)(300*39.37007874);
    infoheader.biXPelsperMeter=0;
    infoheader.biYPelsPerMeter=0;
    infoheader.biClrUsed=0;
    infoheader.biClrlmportant=0;
    BYTE header[BITMAP_HEADER_LEN];
    saveHeader(fileheader, infoheader, header);

    fwrite(header, 54,1,fpout);

    fwrite(data, w*h*bitdepth, 1, fpout);
    fclose(fpout);
    PRINTI("saved to temp.bmp");
}

void Bitmap::saveHeader(BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader, BYTE header[BITMAP_HEADER_LEN]) {
    int index = 0;
    int len = 2;
    memcpy(&header[index], &fileHeader.bfType, len);
    index += len;
    len = 4;
    memcpy(&header[index], &fileHeader.bfSize, len);
    index += len;
    len = 2;
    memcpy(&header[index], &fileHeader.bfReserved1, len);
    index += len;
    len = 2;
    memcpy(&header[index], &fileHeader.bfReserved2, len);
    index += len;
    len = 4;
    memcpy(&header[index], &fileHeader.bfOffBits, len);
    index += len;

    len = 4;
    memcpy(&header[index], &infoHeader.biSize, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biWidth, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biHeight, len);
    index += len;
    len = 2;
    memcpy(&header[index], &infoHeader.biPlanes, len);
    index += len;
    len = 2;
    memcpy(&header[index], &infoHeader.biBitCount, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biCompression, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biSizeImage, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biXPelsperMeter, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biYPelsPerMeter, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biClrUsed, len);
    index += len;
    len = 4;
    memcpy(&header[index], &infoHeader.biClrlmportant, len);
    index += len;
}

void Bitmap::loadFromData(unsigned char* data, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader) {
    int index = 0;
    int len = 2;
    memcpy(&fileHeader.bfType, &data[index], len);
    index += len;
    len = 4;
    memcpy(&fileHeader.bfSize, &data[index], len);
    index += len;
    len = 2;
    memcpy(&fileHeader.bfReserved1, &data[index], len);
    index += len;
    len = 2;
    memcpy(&fileHeader.bfReserved2, &data[index], len);
    index += len;
    len = 4;
    memcpy(&fileHeader.bfOffBits, &data[index], len);
    index += len;

    len = 4;
    memcpy(&infoHeader.biSize, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biWidth, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biHeight, &data[index], len);
    index += len;
    len = 2;
    memcpy(&infoHeader.biPlanes, &data[index], len);
    index += len;
    len = 2;
    memcpy(&infoHeader.biBitCount, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biCompression, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biSizeImage, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biXPelsperMeter, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biYPelsPerMeter, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biClrUsed, &data[index], len);
    index += len;
    len = 4;
    memcpy(&infoHeader.biClrlmportant, &data[index], len);
    index += len;

    PRINTI("bmp w: %d h:%d", infoHeader.biWidth, infoHeader.biHeight);
}

Bitmap::Bitmap() {
    // TODO Auto-generated constructor stub

}

Bitmap::~Bitmap() {
    // TODO Auto-generated destructor stub
}

