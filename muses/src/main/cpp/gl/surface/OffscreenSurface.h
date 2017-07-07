//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_OFFSCREENSURFACE_H
#define MUSES_OFFSCREENSURFACE_H


#include "EglSurfaceBase.h"

class OffscreenSurface : public EglSurfaceBase {
public:
    OffscreenSurface(EglCore *eglCore, int width, int height);

    void release() override;
};


#endif //MUSES_OFFSCREENSURFACE_H
