//
// Created by ranqingguo on 7/12/17.
//

#include <android/native_window_jni.h>

#include "NativeSurfaceTexture.h"
#include "DecodeThread.h"

#undef TAG
#define TAG "NativeSurfaceTexture"

jclass NativeSurfaceTexture::sClass = nullptr;
jmethodID NativeSurfaceTexture::sUpdateMethodId = nullptr;
jmethodID NativeSurfaceTexture::sInitMethodId = nullptr;
jfieldID NativeSurfaceTexture::sHandlerId = nullptr;
jfieldID NativeSurfaceTexture::sSurfaceId = nullptr;

bool NativeSurfaceTexture::initClass(JNIEnv *env) {
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

    auto surfaceId = env->GetFieldID(nstClass, "mSurface", "Landroid/view/Surface;");
    if (surfaceId == nullptr) {
        return false;
    }

    sClass = reinterpret_cast<jclass >(env->NewGlobalRef(nstClass));
    sUpdateMethodId = updateImageMethodId;
    sHandlerId = handlerId;
    sInitMethodId = initId;
    sSurfaceId = surfaceId;

    return true;
}

NativeSurfaceTexture::NativeSurfaceTexture(JNIEnv *env, int textNam) {

//    test(env, textNam);

    auto object = env->NewObject(sClass, sInitMethodId, textNam);
    mTexNam = textNam;
    mJavaInstance = env->NewGlobalRef(object);
    env->SetLongField(mJavaInstance, sHandlerId, (jlong) this);
    mWindow = ANativeWindow_fromSurface(env, env->GetObjectField(mJavaInstance, sSurfaceId));
}

void NativeSurfaceTexture::release(JNIEnv *env) {
    env->DeleteGlobalRef(mJavaInstance);
    ANativeWindow_release(mWindow);
}

void NativeSurfaceTexture::updateImage(JNIEnv *env) {
    env->CallVoidMethod(mJavaInstance, sUpdateMethodId);
}

void NativeSurfaceTexture::onFrameAvailable() {
    auto req = mWaitingReq;
    mWaitingReq = nullptr;
    if (req != nullptr) {
        req->decodeThread->onFrameAvailable(req);
    }
}

void NativeSurfaceTexture::setWaitFrameReady(RenderResRequest *pReq) {
    mWaitingReq = pReq;
}

NativeWindowType NativeSurfaceTexture::getNativeWindow() {
    return mWindow;
}
