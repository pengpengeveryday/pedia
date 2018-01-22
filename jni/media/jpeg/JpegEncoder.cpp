/*
 * JpegEncoder.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: richie
 */

#include "JpegEncoder.h"
#include <cmath>
#include <string>
#include <list>

#define TAG "JpegEncoder"
#include "log.h"

namespace peng {

static const unsigned char testPixel[8][8] = {
        {52,  55,  61,  66,  70,  61,  64,  73},
        {63,  59,  55,  90, 109,  85,  69,  72},
        {62,  59,  68, 113, 144, 104,  66,  73},
        {63,  58,  71, 122, 154, 106,  70,  69},
        {67,  61,  68, 104, 126,  88,  68,  70},
        {79,  65,  60,  70,  77,  68,  58,  75},
        {85,  71,  64,  59,  55,  61,  65,  83},
        {87,  79,  69,  68,  65,  76,  78,  94}
};

static const unsigned char qtable[8][8] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
};

static const int zIndexTable[8][8] = {
        {0, 1, 5, 6, 14, 15, 27, 25},
        {2, 4, 7, 13, 16, 26, 29, 42},
        {3, 8, 12, 17, 25, 30, 41, 43},
        {9, 11, 18, 24, 31, 40, 44, 53},
        {10, 19, 23, 32, 39, 45, 52, 54},
        {20, 22, 33, 38, 46, 51, 55, 60},
        {21, 34, 37, 47, 50, 56, 59, 61},
        {35, 36, 48, 49, 57, 58, 62, 63}
};

JpegEncoder::JpegEncoder() {
    // TODO Auto-generated constructor stub
    encode(testPixel);
}

JpegEncoder::~JpegEncoder() {
    // TODO Auto-generated destructor stub
}

double guv(const unsigned char pixel[8][8], int u, int v) {
    const double pi = acos(-1.0);
    double sum = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            double tempa = (2*x+1)*pi*u/16;
            double tempb = (2*y+1)*v*pi/16;
            double val= (pixel[x][y] - 128)* cos(tempa) * cos(tempb);
            double before = sum;
            sum += val;
        }
    }

    double au = (u == 0 ? 1/std::sqrt(2.0f) : 1);
    double av = (v == 0 ? 1/std::sqrt(2.0f) : 1);
    sum *= au * av /4;
    return sum;
}

void idct(const unsigned char pixel[8][8], double dct[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            dct[i][j] = guv(pixel, i, j);
        }
    }
}

static void quantize(const double idct[8][8], int qresult[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            qresult[i][j] = (idct[i][j]/qtable[i][j] + 0.5f);
        }
    }
}

void retriveTableIndex(int ii, int ij, int& oi, int& oj) {
    int val = zIndexTable[ii][ij];
    oi = val/8;
    oj = val%8;
}

static void zRLE(const int qresult[8][8], int ztable[8][8], std::list<RLEItem>& item) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int oi, oj;
            retriveTableIndex(i, j, oi, oj);
            ztable[oi][oj] = qresult[i][j];
        }
    }

    int zcount = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 0 && j == 0)continue;
            int value = ztable[i][j];
            if (value != 0) {
                RLEItem ri(zcount, value);
                item.push_back(ri);
                zcount = 0;
            } else {
                zcount++;
            }
        }
    }
    if (zcount > 0) {
        RLEItem ri(zcount, 0);
        item.push_back(ri);
    }
}

void JpegEncoder::encode(const unsigned char pixel[8][8]) {
    double idctResult[8][8];
    idct(pixel, idctResult);
    for (int i = 0; i < 8; i++) {
        std::string line = "";
        for (int j = 0; j < 8; j++) {
            char temp[256] = {0};
            sprintf(temp, "%lf ", idctResult[i][j]);
            line += temp;
        }
        PRINTI("%s", line.c_str());
    }

    int qresult[8][8];
    quantize(idctResult, qresult);
    PRINTI("after quantized");
    for (int i = 0; i < 8; i++) {
        std::string line = "";
        for (int j = 0; j < 8; j++) {
            char temp[256] = {0};
            sprintf(temp, "%d ", qresult[i][j]);
            line += temp;
        }
        PRINTI("%s", line.c_str());
    }

    int ztable[8][8];
    std::list<RLEItem> items;
    zRLE(qresult, ztable, items);
    PRINTI("after index ztable");
    for (int i = 0; i < 8; i++) {
        std::string line = "";
        for (int j = 0; j < 8; j++) {
            char temp[256] = {0};
            sprintf(temp, "%d ", ztable[i][j]);
            line += temp;
        }
        PRINTI("%s", line.c_str());
    }

    std::string zrle;
    for (std::list<RLEItem>::iterator it = items.begin(); it != items.end(); it++) {
        RLEItem ri = *it;
        char buff[256] = {0};
        sprintf(buff, "(%d, %d), ", ri.zeroCount, ri.value);
        zrle += buff;
    }
    PRINTI("%s", zrle.c_str());
}

}

int main() {
    PRINTI("hello world");
    peng::JpegEncoder encoder;
    return 0;
}
