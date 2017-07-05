#include <jni.h>
#include <string>

#include <thread>

#include "gl/EglCore.h"
#include "util/LogUtil.h"

#undef TAG
#define TAG "native-lib"

extern "C"
JNIEXPORT jstring


JNICALL
Java_rqg_fantasy_pbotest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


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


    return env->NewStringUTF(hello.c_str());
}
