#include <jni.h>
#include <string>

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_example_pedia_MediaJni_sayHello(JNIEnv *env, jclass)
{
    std::string msg;
    return env->NewStringUTF("Hello JNI!");
}

}
