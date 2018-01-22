/*
 * BmpParser.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#include "BmpExtractor.h"
#define TAG "Extractor"
#include "log.h"
#include "Bitmap.h"

namespace peng {

BmpExtractor::BmpExtractor(DataSource& source) : mSource(source), mBitDepth(BMP_UNSUPPORT) {
    // TODO Auto-generated constructor stub
    PRINTI("filesize: %llu", mSource.getSize());
    BYTE* data = new BYTE[mSource.getSize()];
    source.readAt(0, data, mSource.getSize());
    Bitmap::BITMAPFILEHEADER fheader;
    Bitmap::BITMAPINFOHEADER iheader;
    Bitmap::loadFromData(data, fheader, iheader);

    mWidth = iheader.biWidth;
    mHeight = iheader.biHeight;
    mBitCount = iheader.biBitCount;
    //Bitmap::save2File(&data[Bitmap::BITMAP_HEADER_LEN], mWidth, mHeight, bitCount/8);
    mData = data;
    if (mBitCount == 16) {
        mBitDepth = BMP_565;
    } else if (mBitCount == 24) {
        mBitDepth = BMP_888;
    }
}

BmpExtractor::~BmpExtractor() {
    // TODO Auto-generated destructor stub
    delete[] mData;
}

static void bmp2yuv(unsigned char rgb[3], unsigned char yuv[3]) {
    double Y, U, V;
    BYTE R, G, B;

    R = rgb[0];
    G = rgb[1];
    B = rgb[2];
    Y = 0.299*R + 0.587*G + 0.114*B;
    U = -0.1687*R - 0.3313*G + 0.5*B + 128;
    V = 0.5*R - 0.4187*G - 0.0813*B + 128;
    yuv[0] = (BYTE)Y > 235?235:(BYTE)Y;
    yuv[1] = (BYTE)U > 240?240:(BYTE)U;
    yuv[2] = (BYTE)V > 240?240:(BYTE)V;
    yuv[0] = (BYTE)Y<16?16:(BYTE)Y;
    yuv[1] = (BYTE)U<16?16:(BYTE)U;
    yuv[2] = (BYTE)V<16?16:(BYTE)V;
    yuv[0] = Y;
    yuv[1] = U;
    yuv[2] = V;
}

void BmpExtractor::bmp2yuv420(unsigned char* const yuv, int size) {
    if (mWidth*mHeight*3/2 != size) {
        PRINTE("cache size not pair");
        return;
    }

    int pixelByte = mBitCount/8;
    int linePixelWidth = mWidth*pixelByte;
    int YLEN = mWidth*mHeight;
    int ULEN = mWidth*mHeight/4;
    unsigned char* const y = yuv;
    unsigned char* const u = y + YLEN;
    unsigned char* const v = u + ULEN;
    BYTE yuvpixel[3];
    for (int h = 0; h < mHeight; h++) {
        int pos = h*linePixelWidth;
        for (int i = 0; i < mWidth; i++) {
            bmp2yuv(&mData[pos + i*pixelByte], yuvpixel);
            y[h*mWidth + i] = yuvpixel[0];
            if (i%2 == 0 && h%2 == 0) {
                u[h*mWidth/4 + i/2] = yuvpixel[1];
                v[h*mWidth/4 + i/2] = yuvpixel[2];
            }
        }
    }
}

}

