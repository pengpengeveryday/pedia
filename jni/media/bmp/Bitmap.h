/*
 * Bitmap.h
 *
 *  Created on: Jan 22, 2018
 *      Author: richie
 */

#ifndef BITMAP_H_
#define BITMAP_H_

typedef unsigned char   BYTE;
typedef unsigned short int  WORD;
typedef unsigned int   DWORD;

class Bitmap {
public:
    enum { BITMAP_HEADER_LEN = 54 };
    /**/
    // 14 bytes
    typedef struct tagBITMAPFILEHEADER
    {
        WORD bfType;            /**/
        DWORD bfSize;           /* byte count*/
        WORD bfReserved1;       /**/
        WORD bfReserved2;       /**/
        DWORD bfOffBits;        /*accurate offset to data*/
    } BITMAPFILEHEADER;

    /**/
    // 40 bytes
    typedef struct tagBITMAPINFOHEADER
    {
      DWORD biSize;           /*header size count*/
      DWORD biWidth;          /*width pixel*/
      DWORD biHeight;         /*height pixel*/
      WORD  biPlanes;         /* 1 */
      WORD  biBitCount;       /*bit count*/
      DWORD biCompression;    /**/
      DWORD biSizeImage;      /*picture size*/
      DWORD biXPelsperMeter;  /**/
      DWORD biYPelsPerMeter;  /**/
      DWORD biClrUsed;        /**/
      DWORD biClrlmportant;   /**/
    } BITMAPINFOHEADER;

    static void save2File(unsigned char* data, int w, int h, int bitdepth);
    static void loadFromData(unsigned char* data, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader);

    Bitmap();
    virtual ~Bitmap();

protected:
    static void saveHeader(BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader, BYTE header[BITMAP_HEADER_LEN]);

private:
    BYTE mHeader[BITMAP_HEADER_LEN];
};

#endif /* BITMAP_H_ */
