//
// Created by ranqingguo on 7/12/17.
//

#ifndef PBOTEST_NATIVESURFACETEXTURE_H
#define PBOTEST_NATIVESURFACETEXTURE_H

#include <android/bitmap.h>
#include <jni.h>
#include <EGL/eglplatform.h>

struct RenderResRequest;

class NativeSurfaceTexture {
public:
    NativeSurfaceTexture(JNIEnv *env, int textNam);

    void updateImage(JNIEnv *env);

    void release(JNIEnv *env);

    void onFrameAvailable();

    void setWaitFrameReady(RenderResRequest *pReq);

    NativeWindowType getNativeWindow();

private:
    jobject mJavaInstance;
    RenderResRequest *mWaitingReq;
    NativeWindowType mWindow;
    int mTexNam;


public:
    static bool initClass(JNIEnv *env);

private:
    static jclass sClass;
    static jmethodID sUpdateMethodId;
    static jmethodID sInitMethodId;
    static jfieldID sHandlerId;
    static jfieldID sSurfaceId;
};


#endif //PBOTEST_NATIVESURFACETEXTURE_H
