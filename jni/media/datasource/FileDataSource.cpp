/*
 * FileDataSource.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#include "FileDataSource.h"
#define TAG "DataSource"
#include "log.h"

namespace peng {

FileDataSource::FileDataSource(std::string filepath) : mFP(NULL) {
    // TODO Auto-generated constructor stub
    FILE* fp = fopen(filepath.c_str(), "rb");
    if (fp == NULL) {
        PRINTE("read file failed");
        return;
    }
    mFP = fp;
    fseek(mFP, 0, SEEK_END);
    mSize = ftell(mFP);
    fseek(mFP, 0, SEEK_SET);
}

FileDataSource::~FileDataSource() {
    // TODO Auto-generated destructor stub
}

uint64_t FileDataSource::readAt(uint64_t offset, void *data, uint64_t size) {
    fseek(mFP, offset, SEEK_SET);
    int readSize = size;
    if (offset + readSize > mSize) {
        readSize = mSize - offset;
    }
    return fread(data, readSize, 1, mFP);
}

uint64_t FileDataSource::getSize() {
    return mSize;
}

}
