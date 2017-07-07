//
// Created by ranqingguo on 7/5/17.
//

#include "WindowSurface.h"


void WindowSurface::release() {
    releaseEglSurface();
    if (mWindow != nullptr && mReleaseWindow) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
}

WindowSurface::WindowSurface(EglCore *eglCore, NativeWindowType window, bool releaseWindow)
        : EglSurfaceBase(eglCore), mWindow(window), mReleaseWindow(releaseWindow) {

}
