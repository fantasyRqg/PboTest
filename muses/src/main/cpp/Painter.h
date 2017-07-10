//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_PAINTER_H
#define PBOTEST_PAINTER_H

#include "util/Looper.h"
#include "RenderTask.h"
#include "gl/EglCore.h"
#include "gl/surface/EglSurfaceBase.h"


class Uploader;

/**
 * response for draw frame
 */
class Painter : public Looper {
public:
    Painter();

    virtual ~Painter();

    void postDrawRenderTask(RenderTask *task);


    void handleDrawRenderTask(RenderTask *pTask);

    void bindUploader(Uploader *uploader);

    void quit() override;

private:
    Uploader *mUploader;
    EglCore *mEglCore;
    EglSurfaceBase *mEglSurface;

    void handle(int what, void *data) override;

    void handleStart();

    void handleCreateWindowSurface(NativeWindowType pWindow);

    void handleCreateOffScreenSurface();

    void handleDestroySurface();

    void handleStop();
};


#endif //PBOTEST_PAINTER_H
