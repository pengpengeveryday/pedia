#include "log.h"
#include "BmpExtractor.h"
#include "datasource/FileDataSource.h"

using namespace peng;
#define TAG "MAIN"

int main() {
    PRINTI("HELLO WOLRD");
    FileDataSource source("/sdcard/bgy.bmp");
    BmpExtractor extractor(source);

    int size = extractor.width() * extractor.height() * 3 / 2;
    unsigned char* pYUV = new unsigned char[size];
    extractor.bmp2yuv420(pYUV, size);
    FILE* fp = fopen("temp.yuv", "wb");
    fwrite(pYUV, size, 1, fp);
    fclose(fp);
    delete[] pYUV;

    return 0;
}
