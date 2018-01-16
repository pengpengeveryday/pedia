#ifndef _LOCK_H_
#define _LOCK_H_

#include <pthread.h>

#ifndef PRINT
#include <stdio.h>
#define PRINT(format, args...)  {              \
                printf(format, ##args);        \
                printf("\n");                  \
        }
#endif


namespace peng {

class Lock {
public:
    Lock();
    ~Lock();

    void lock();

    void unlock();
private:
    pthread_mutex_t mutex_;
};

class AutoLock {
public:
    AutoLock(Lock& l);
    ~AutoLock();
private:
    Lock& l_;
};

}


#endif
