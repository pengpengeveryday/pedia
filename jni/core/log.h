/*
 * log.h
 *
 *  Created on: Jan 19, 2018
 *      Author: richie
 */

#ifndef LOG_H_
#define LOG_H_

#ifndef PRINT
#include <stdio.h>
#ifdef TAG
#define PRINT(level, format, args...)  {              \
                printf("[%s] ", TAG); \
                printf("%c: ", level); \
                printf(format, ##args);        \
                printf("\n");                  \
        }
#else
#define PRINT(level, format, args...)  {              \
                printf("%c: ", level); \
                printf(format, ##args);        \
                printf("\n");                  \
        }
#endif
#endif

#ifndef PRINTW
#define PRINTW(format, args...) PRINT('W', format, ##args)
#endif

#ifndef PRINTE
#define PRINTE(format, args...) PRINT('E', format, ##args)
#endif

#ifndef PRINTI
#define PRINTI(format, args...) PRINT('I', format, ##args)
#endif

#ifndef CHECK
#define ASSERT(exp) assert(exp)
#endif

#ifndef CHECK_EQ
#define CHECK_EQ(a, b) ASSERT((a) == (b))
#endif

#endif /* LOG_H_ */
