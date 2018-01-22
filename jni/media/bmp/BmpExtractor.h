/*
 * BmpParser.h
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#ifndef BMPEXTRACTOR_H_
#define BMPEXTRACTOR_H_

#include "datasource/DataSource.h"

namespace peng {

class BmpExtractor {
public:
    enum {
        BMP_UNSUPPORT = -1, BMP_565, BMP_888
    };

    BmpExtractor(DataSource& source);
    virtual ~BmpExtractor();

    int width() { return mWidth; }
    int height() { return mHeight; }

    void bmp2yuv420(unsigned char* const yuv, int size);

private:
    DataSource& mSource;
    unsigned char* mData;
    int mWidth;
    int mHeight;
    int mBitDepth;
    int mBitCount;
};


}

#endif /* BMPPARSER_H_ */
