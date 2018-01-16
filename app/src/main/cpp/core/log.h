#ifndef _LOG_H_
#define _LOG_H_

#ifndef PRINT
#include <stdio.h>
#define PRINT(format, args...)  {              \
                printf(format, ##args);        \
                printf("\n");                  \
        }
#endif



#endif
