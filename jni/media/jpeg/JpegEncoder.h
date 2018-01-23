/*
 * JpegEncoder.h
 *
 *  Created on: Jan 22, 2018
 *      Author: richie
 */

#ifndef JPEGENCODER_H_
#define JPEGENCODER_H_

namespace peng {

typedef struct tagRLEItem {
    int zeroCount;
    int value;

    tagRLEItem(int zcount, int val) {
        zeroCount = zcount;
        value = val;
    }
} RLEItem;

typedef struct tagQItem {
    int zeroCount;
    unsigned short code;
    int size;
    unsigned short code2;

    tagQItem(int zc, unsigned short c, int sz) {
        zeroCount = zc;
        code = c;
        size = sz;
        code2 = 0;
    }
} QItem;


class JpegEncoder {
public:
    JpegEncoder();
    virtual ~JpegEncoder();

    void encode(const unsigned char pixel[8][8]);

private:

};

}

#endif /* JPEGENCODER_H_ */
