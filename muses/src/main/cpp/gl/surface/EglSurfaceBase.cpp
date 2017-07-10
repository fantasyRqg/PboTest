//
// Created by ranqingguo on 7/5/17.
//

#include "EglSurfaceBase.h"
#include "../../exception/IlleaglStateException.h"

#define SURFACE_EXIST "surface already created"

EglSurfaceBase::EglSurfaceBase(EglCore *eglCore) : mEglCore(eglCore) {}

void EglSurfaceBase::createWindowSurface(EGLNativeWindowType window) {
    if (mEglSurface != EGL_NO_SURFACE) {
        throw IllegalStateException(SURFACE_EXIST);
    }

    mEglSurface = mEglCore->createWindowSurface(window);
}

void EglSurfaceBase::createOffscreenSurface(int width, int height) {
    if (mEglSurface != EGL_NO_SURFACE) {
        throw IllegalStateException(SURFACE_EXIST);
    }

    mEglSurface = mEglCore->createOffscreenSurface(width, height);

    mWidth = width;
    mHeight = height;
}

int EglSurfaceBase::getWidth() const {
    if (mWidth < 0) {
        return mEglCore->querySurface(mEglSurface, EGL_WIDTH);
    } else {
        return mWidth;
    }
}

int EglSurfaceBase::getHeight() const {
    if (mHeight < 0) {
        return mEglCore->querySurface(mEglSurface, EGL_HEIGHT);
    } else {
        return mHeight;
    }
}

void EglSurfaceBase::releaseEglSurface() {
    mEglCore->releaseSurface(mEglSurface);
    mEglSurface = EGL_NO_SURFACE;
    mWidth = mHeight = -1;

    mEglCore->makeNothingCurrent();
}

void EglSurfaceBase::makeCurrent() {
    mEglCore->makeCurrent(mEglSurface);
}

bool EglSurfaceBase::swapBuffers() {
    return mEglCore->swapBuffers(mEglSurface);
}

void EglSurfaceBase::setPresentationTime(long timeUs) {
    mEglCore->presentationTimeANDROID(mEglSurface, timeUs);
}

float EglSurfaceBase::getAspect() {
    return float(getWidth()) / float(getHeight());
}

EglSurfaceBase::~EglSurfaceBase() {

}

