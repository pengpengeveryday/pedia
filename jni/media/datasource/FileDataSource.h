/*
 * FileDataSource.h
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#ifndef FILEDATASOURCE_H_
#define FILEDATASOURCE_H_

#include "DataSource.h"
#include <string>

namespace peng {

class FileDataSource : public DataSource {
public:
    FileDataSource(std::string filepath);
    virtual ~FileDataSource();

    virtual uint64_t readAt(uint64_t offset, void *data, uint64_t size);

    virtual uint64_t getSize();

private:
    uint64_t mSize;
    FILE* mFP;
};

}

#endif /* FILEDATASOURCE_H_ */
