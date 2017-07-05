#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <thread>

#include "gl/EglCore.h"
#include "util/common.h"

#undef TAG
#define TAG "native-lib"

extern "C"
JNIEXPORT jstring


JNICALL
Java_rqg_fantasy_pbotest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_rqg_fantasy_pbotest_MainActivity_testMuses(JNIEnv *env, jobject instance,
                                                jobject assetManager) {

    AAssetManager *as = AAssetManager_fromJava(env, assetManager);


    new std::thread([]() {
        EglCore eglCore;
        LOGD("egl setup");

        try {
            eglCore.setUp();
        } catch (std::runtime_error e) {
            LOGE("egl core setup error : %s", e.what());
            return;
        }

        eglCore.makeCurrent(EGL_NO_SURFACE);

        eglCore.logGlInfo();

        LOGD("egl tearDown");
        eglCore.tearDown();
    });

}