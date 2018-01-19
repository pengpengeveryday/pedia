#include "log.h"
#include "BmpExtractor.h"
#include "datasource/FileDataSource.h"

using namespace peng;

int main() {
    PRINTI("HELLO WOLRD");
    FileDataSource source("/sdcard/bgy.bmp");
    BmpExtractor extractor(source);

    return 0;
}
