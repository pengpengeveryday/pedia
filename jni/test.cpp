#include <jni.h>

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_example_pedia_MediaJni_sayHello(JNIEnv *env, jclass)
{
    return env->NewStringUTF("Hello JNI!");
}

}
