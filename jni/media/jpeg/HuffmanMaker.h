/*
 * HuffmanMaker.h
 *
 *  Created on: Jan 23, 2018
 *      Author: richie
 */

#ifndef HUFFMANMAKER_H_
#define HUFFMANMAKER_H_

class HuffmanMaker {
public:
    enum { ROW_COUNT = 16 };
    HuffmanMaker();
    virtual ~HuffmanMaker();

    int make(short value, unsigned short & code);

protected:
    int getRow(int value);

private:
    int mRowRange[ROW_COUNT];
};

#endif /* HUFFMANMAKER_H_ */
