#include <jni.h>
#include <string>
#include "MessageQueue.h"
using namespace peng;

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_peng_pedia_MainActivity_stringFromJNI (
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    MessageQueue::Message msg;
    return env->NewStringUTF(hello.c_str());
}
