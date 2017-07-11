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
#include "effect/TestEffect.h"
#include "util/common.h"

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
    Player *player = new Player();
    Uploader *uploader = new Uploader(painter, decodeThread, player, 4);

    painter->postCreateWindowSurface(pWindow);


    EffectLine *el = new EffectLine();
    try {
        el->append(new TestEffect(100000000L));
    } catch (std::runtime_error e) {
        LOGE("init effect error: %s", e.what());
        return;
    }

    EffectManager *em = new EffectManager(24, el);

    player->play(em);

//    painter->quit();
//    uploader->quit();
//    decodeThread->quit();
}