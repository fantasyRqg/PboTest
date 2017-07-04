//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_EGLCORE_H
#define PBOTEST_EGLCORE_H


#include <vector>
#include <set>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <memory>
#include <android/native_window.h>
#include <android/asset_manager.h>
#include <android/log.h>

class EglCore {
public:
    EglCore(EGLNativeWindowType window, bool offScreen = false);


    ~EGLWrapper();


    bool setUp();

    bool tearDown();

    bool swapBuffers();

    /**
     * get surface size
     * @param size size[0] = width,size[1] = height
     */
    bool querySurfaceSize(int *size);

protected:
    bool mOffScreen;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
    EGLConfig mGlConfig;
    EGLNativeWindowType mWindow;

};


#endif //PBOTEST_EGLCORE_H
