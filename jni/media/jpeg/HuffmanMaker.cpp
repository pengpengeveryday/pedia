/*
 * HuffmanMaker.cpp
 *
 *  Created on: Jan 23, 2018
 *      Author: richie
 */

#include "HuffmanMaker.h"
#include <cmath>
#include <string>

#define TAG "HuffmanMaker"
#include "log.h"

HuffmanMaker::HuffmanMaker() {
    // TODO Auto-generated constructor stub
    char buff[256] = {0};
    std::string log;
    for (int i = 0; i < ROW_COUNT; i++) {
        mRowRange[i] = pow(2.0f, i);
        sprintf(buff, "%d ", mRowRange[i]);
        log += buff;
    }
    //PRINTI("%s", log.c_str());
}

HuffmanMaker::~HuffmanMaker() {
    // TODO Auto-generated destructor stub
}

int HuffmanMaker::getRow(int value) {
    for (int i = 0; i < ROW_COUNT; i++) {
        if (value < mRowRange[i]) return i;
    }
    return -1;
}

short generate(int length) {
    short value = 0;

    while (length > 0) {
        value = (value|0x01);
        value = value<<1;
        length--;
    }
    return value;
}

int HuffmanMaker::make(short value, unsigned short & code) {
    code = 0;
    if (value == 0) return 0;

    int absv = (value >= 0 ? value : -value);
    int rowIndex = getRow(absv);

    if (value > 0) {
        code = value;
    } else {
        // < 0 path
        short temp = (-value);
        bool first = true;
        for (int i = rowIndex - 1; i >= 0; i--) {
            code = code<<1;
            short bitValue = (temp>>i &0x01);
            bitValue = bitValue == 1 ? 0 : 1;
            if (!first) {
                code += bitValue;
            } else {
                first = false;
            }
        }
    }

    PRINTI("%d -> %d, code len:%d", value, code, rowIndex);

    return rowIndex;
}
