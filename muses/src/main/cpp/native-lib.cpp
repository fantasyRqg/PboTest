#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <thread>
#include <android/native_window.h>
#include <android/native_window_jni.h>


#include "gl/EglCore.h"
#include "framesource/DecodeThread.h"
#include "Painter.h"
#include "Uploader.h"

#undef TAG
#define TAG "native-lib"


extern "C"
JNIEXPORT jstring JNICALL
Java_rqg_fantasy_muses_Native_stringFromJNI(JNIEnv *env, jclass type) {

    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_rqg_fantasy_muses_Native_testMuses(JNIEnv *env, jclass type, jobject assetManager) {

    AAssetManager *as = AAssetManager_fromJava(env, assetManager);


}extern "C"
JNIEXPORT void JNICALL
Java_rqg_fantasy_muses_Native_onSurfaceCreated(JNIEnv *env, jclass type, jobject assetManager,
                                               jobject surface) {

    AAssetManager *as = AAssetManager_fromJava(env, assetManager);
    auto pWindow = ANativeWindow_fromSurface(env, surface);

    DecodeThread *decodeThread = new DecodeThread();
    Painter *painter = new Painter(as);
    painter->postCreateWindowSurface(pWindow);
    Player *player = new Player();
    Uploader *uploader = new Uploader(painter, decodeThread, player, 4);


//    painter->quit();
//    uploader->quit();
//    decodeThread->quit();
}