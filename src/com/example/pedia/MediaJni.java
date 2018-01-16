package com.example.pedia;

public class MediaJni {

    static {
        System.loadLibrary("pedia-jni");
    }

    static public native String sayHello();
}
