/*
 * DataSource.h
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

#include <sys/types.h>

namespace peng {


class DataSource {
public:
    DataSource();
    virtual ~DataSource();

    virtual uint64_t readAt(uint64_t offset, void *data, uint64_t size) = 0;

    virtual uint64_t getSize();
};

}

#endif /* DATASOURCE_H_ */
