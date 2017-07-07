//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_WINDOWSURFACE_H
#define MUSES_WINDOWSURFACE_H


#include "EglSurfaceBase.h"

class WindowSurface : public EglSurfaceBase {
public:
    WindowSurface(EglCore *eglCore, NativeWindowType window, bool releaseWindow);

    void release() override;

private:
    NativeWindowType mWindow;
    bool mReleaseWindow;
};


#endif //MUSES_WINDOWSURFACE_H
