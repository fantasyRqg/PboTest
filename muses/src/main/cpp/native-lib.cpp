#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>


#include "gl/EglCore.h"
#include "util/common.h"
#include "NativeSurfaceTexture.h"
#include "EffectLine.h"
#include "effect/TestEffect.h"
#include "EffectManager.h"
#include "Player.h"
#include "effect/NormalEffect.h"

#undef TAG
#define TAG "native-lib"

static JavaVM *jvm;

bool tttest(JNIEnv *env, int texNam) {
    auto nstClass = env->FindClass("rqg/fantasy/muses/NativeSurfaceTexture");
    if (nstClass == nullptr) {
        return false;
    }

    auto updateImageMethodId = env->GetMethodID(nstClass, "updateTexImage", "()V");
    if (updateImageMethodId == nullptr) {
        return false;
    }

    auto initId = env->GetMethodID(nstClass, "<init>", "(I)V");
    if (initId == nullptr) {
        return false;
    }

    auto handlerId = env->GetFieldID(nstClass, "mNativeHandler", "J");
    if (handlerId == nullptr) {
        return false;
    }

    auto object = env->NewObject(nstClass, initId, texNam);
    auto jst = env->NewGlobalRef(object);
    for (int i = -10; i < 100; ++i) {
        env->SetLongField(jst, handlerId, (jlong) i);
        auto hv = env->GetLongField(jst, handlerId);
        LOGD("get handler = %lld", hv);
    }

    return false;
}

void onSurfaceCreated(JNIEnv *env, jclass type, jobject surface, jobject assetManager) {

//    tttest(env, 0);

    AAssetManager *as = AAssetManager_fromJava(env, assetManager);
    auto pWindow = ANativeWindow_fromSurface(env, surface);

    DecodeThread *decodeThread = new DecodeThread();
    Painter *painter = new Painter(as, jvm);
    Player *player = new Player(decodeThread, painter);

    painter->postCreateWindowSurface(pWindow);


    EffectLine *el = new EffectLine();
    try {
        el->append(new TestEffect(3000000L));
        el->append(new NormalEffect("/sdcard/testVideo/test3.mp4", 1000000L));
        el->append(new NormalEffect("/sdcard/testVideo/test2.mp4", 2000000L));
        el->append(new NormalEffect("/sdcard/testVideo/test1.mp4", 1000000L));
    } catch (std::runtime_error e) {
        LOGE("init effect error: %s", e.what());
        return;
    }

    LOGD("el info = %s", el->getInfo().c_str());

    EffectManager *em = new EffectManager(24, el);

    player->play(em);
}

jboolean nativeInit(JNIEnv *env, jclass type) {
    auto status = env->GetJavaVM(&jvm);
    if (status != 0) {
        LOGE("get java vm failure");

        return JNI_FALSE;
    }

    return (jboolean) NativeSurfaceTexture::initClass(env);
}

void onFrameAvailable(JNIEnv *env, jclass type, jlong handler) {
    ((NativeSurfaceTexture *) handler)->onFrameAvailable();
}

//------------------------------------------------------------------------------------------------------------
static JNINativeMethod gGLES3JniViewMethods[] = {
        {"onSurfaceCreated", "(Landroid/view/Surface;Landroid/content/res/AssetManager;)V", (void *) onSurfaceCreated},
        {"onFrameAvailable", "(J)V",                                                        (void *) onFrameAvailable},
        {"nativeInit",       "()Z",                                                         (void *) nativeInit},
};

static const char *classPathName = "rqg/fantasy/muses/MusesNative";


/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("MusesNative registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv *env) {
    if (!registerNativeMethods(env, classPathName,
                               gGLES3JniViewMethods,
                               sizeof(gGLES3JniViewMethods) / sizeof(gGLES3JniViewMethods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv *env;
    void *venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv *env = NULL;

    LOGI("JNI_OnLoad");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }
    result = JNI_VERSION_1_6;
    bail:
    return result;
}



