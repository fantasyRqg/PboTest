//
// Created by ranqingguo on 7/5/17.
//

#ifndef PBOTEST_EGLSURFACEBASE_H
#define PBOTEST_EGLSURFACEBASE_H

#include "../EglCore.h"

class EglSurfaceBase {
public:
    EglSurfaceBase(EglCore *eglCore);

    int getWidth() const;

    int getHeight() const;

    void releaseEglSurface();

    void makeCurrent();

    bool swapBuffers();

    /**
     * set frame present time
     * @param nanoseconds unit nanoseconds
     */
    void setPresentationTime(long nanoseconds);

    virtual void release() = 0;

protected:
    void createWindowSurface(EGLNativeWindowType window);

    void createOffscreenSurface(int width, int height);

    EglCore *mEglCore;
    int mWidth = -1;
    int mHeight = -1;
    EGLSurface mEglSurface = EGL_NO_SURFACE;
};


#endif //PBOTEST_EGLSURFACEBASE_H
