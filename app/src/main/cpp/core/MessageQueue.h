#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include "SingleThread.h"
#include "Semaphore.h"
#include <list>

namespace peng {

class MessageQueue: public SingleThread::ThreadProc {
public:

typedef struct {
    int what;
    int arg1;
    int arg2;
    void* data;
} Message;

class MessageCallback {
public:
    MessageCallback() {}
    virtual ~MessageCallback() {}
    virtual void messageReceived(const Message& msg) = 0;
};

    MessageQueue();
    ~MessageQueue();

    void post(Message& msg);
    void postDelay(Message& msg, unsigned int delayMS);
    void registerCallback(MessageCallback& cb);
    void reset();


protected:
    // inherit from SingleThread::ThreadProc
    virtual bool process(int thread_id);
private:
    std::list<Message> msgs_;
    Lock l_;
    Semaphore sem_;
    MessageCallback* cb_;
    SingleThread thread_;
    bool running_;
};

}

#endif
