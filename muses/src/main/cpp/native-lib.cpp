#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <thread>

#include "gl/EglCore.h"
#include "util/common.h"

#include "render/PboTestRender.h"

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


    new std::thread([as]() {
        LOGD("egl setup");
        EglCore *eglCore;
        try {
            eglCore = new EglCore();
        } catch (std::runtime_error e) {
            LOGE("egl core setup error : %s", e.what());
            return;
        }

        eglCore->makeCurrent(EGL_NO_SURFACE);

        eglCore->logGlInfo();

        PboTestRender render;

        render.setUp(as);

        render.drawFrame(0);

        render.tearDown();
        LOGD("egl tearDown");
        eglCore->tearDown();

        delete eglCore;
    });

}