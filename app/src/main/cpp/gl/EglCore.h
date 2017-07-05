//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_EGLCORE_H
#define PBOTEST_EGLCORE_H

#ifndef EGL_EGLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif //EGL_EGLEXT_PROTOTYPES

#include <vector>
#include <set>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <memory>
#include <android/native_window.h>
#include <android/asset_manager.h>
#include <android/log.h>
#include <string>

class EglCore {

public:
    EglCore();

    ~EglCore();

    bool setUp();

    bool tearDown();


    void releaseSurface(EGLSurface eglSurface);

    EGLSurface createWindowSurface(NativeWindowType window);

    EGLSurface createOffscreenSurface(int width, int height);

    void makeCurrent(EGLSurface eglSurface);

    bool swapBuffers(EGLSurface eglSurface);

    int querySurface(EGLSurface eglSurface, int what);

    /**
     * set frame present time
     * @param nanoseconds unit nanoseconds
     */
    void presentationTimeANDROID(EGLSurface eglSurface, long nanoseconds);

protected:
    bool mOffScreen;
    EGLDisplay mEglDisplay;
    EGLContext mEglContext;
    EGLConfig mGlConfig;
    EGLNativeWindowType mWindow;


private:
    void checkEglError(std::string msg);

};


#endif //PBOTEST_EGLCORE_H