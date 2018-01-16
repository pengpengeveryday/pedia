#include "MessageQueue.h"


namespace peng {


MessageQueue::MessageQueue(): sem_(0) {
    thread_.registerThreadProc(*this);
    thread_.start();
}

MessageQueue::~MessageQueue() {
    msgs_.clear();
    sem_.post();
    thread_.stop();
}

bool MessageQueue::process(int thread_id) {
    sem_.wait();

    {
        AutoLock l(l_);
        if (msgs_.size() > 0) {
            Message msg = msgs_.front();
            msgs_.pop_front();

            if (cb_) {
                cb_->messageReceived(msg);
            }

        } else {
            return false;
        }
    }

    return true;
}

void MessageQueue::registerCallback(MessageQueue::MessageCallback& cb) {
    cb_ = &cb;
}

void MessageQueue::post(Message& msg) {
    AutoLock l(l_);
    msgs_.push_back(msg);
    sem_.post();
}

void MessageQueue::reset() {
    AutoLock l(l_);
    msgs_.clear();
}

}
