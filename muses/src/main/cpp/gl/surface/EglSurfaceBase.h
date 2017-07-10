//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_EGLSURFACEBASE_H
#define MUSES_EGLSURFACEBASE_H

#include "../EglCore.h"

class EglSurfaceBase {
public:
    EglSurfaceBase(EglCore *eglCore);

    virtual ~EglSurfaceBase();

    int getWidth() const;

    int getHeight() const;


    void makeCurrent();

    bool swapBuffers();

    /**
     * set frame present time
     * @param nanoseconds unit nanoseconds
     */
    void setPresentationTime(long nanoseconds);

    virtual void release() = 0;

    float getAspect();

    EGLSurface getEglSurface() const;

protected:
    void releaseEglSurface();

    void createWindowSurface(EGLNativeWindowType window);

    void createOffscreenSurface(int width, int height);

    EglCore *mEglCore;
    int mWidth = -1;
    int mHeight = -1;
    EGLSurface mEglSurface = EGL_NO_SURFACE;
};


#endif //MUSES_EGLSURFACEBASE_H
