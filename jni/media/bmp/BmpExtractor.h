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
    BmpExtractor(DataSource& source);
    virtual ~BmpExtractor();

private:
    DataSource& mSource;
};


}

#endif /* BMPPARSER_H_ */
