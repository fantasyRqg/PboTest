//
// Created by ranqingguo on 7/5/17.
//

#include "OffscreenSurface.h"


void OffscreenSurface::release() {
    releaseEglSurface();
}

OffscreenSurface::OffscreenSurface(EglCore *eglCore, int width, int height)
        : EglSurfaceBase(eglCore) {
    createOffscreenSurface(width, height);
}

