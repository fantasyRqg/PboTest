//
// Created by ranqingguo on 7/7/17.
//

#include "Painter.h"
#include "Uploader.h"
#include "gl/surface/WindowSurface.h"

enum {
    kWhatStart,
    kWhatStopCurrent,
    kWhatDrawRenderTask,
    kWhatCreateWindowSurface,
    kWhatCreateOffScreenSurface,
    kWhatDestroySurface,
    kWhatRenderSetup,
    kWhatRenderTearDown,
};

void Painter::postDrawRenderTask(RenderTask *task) {
    post(kWhatDrawRenderTask, task);
}

void Painter::handle(int what, void *data) {

    switch (what) {
        case kWhatStart:
            handleStart();
            break;

        case kWhatStopCurrent:
            handleStop();
            break;
        case kWhatDrawRenderTask:
            handleDrawRenderTask((RenderTask *) data);
            break;


        case kWhatCreateOffScreenSurface:
            handleCreateOffScreenSurface();
            break;

        case kWhatCreateWindowSurface:
            handleCreateWindowSurface((NativeWindowType) data);
            break;

        case kWhatDestroySurface:
            handleDestroySurface();
            break;

        default:
            break;
    }
}

void Painter::handleDrawRenderTask(RenderTask *pTask) {
    pTask->draw();

    mEglSurface->swapBuffers();

    int size = pTask->getFrameSourceVector().size();


    for (int i = 0; i < size; ++i) {
        mUploader->releaseBuffer(pTask->getPboResAt(i));
    }
}

void Painter::bindUploader(Uploader *uploader) {
    mUploader = uploader;
    post(kWhatStart, nullptr);
}

Painter::Painter() {

}

void Painter::handleStart() {
    mEglCore = new EglCore;
    mUploader->startUploader(mEglCore->getEglContext());
}

void Painter::handleStop() {
    if (mEglCore != nullptr) {
        mEglCore->tearDown();
        delete mEglCore;
    }

}

void Painter::quit() {
    post(kWhatStopCurrent, nullptr);
    Looper::quit();
}

void Painter::handleCreateWindowSurface(NativeWindowType pWindow) {
    mEglSurface = new WindowSurface(mEglCore, pWindow, true);
    mEglCore->makeCurrent(mEglSurface);
}

void Painter::handleCreateOffScreenSurface() {

}

void Painter::handleDestroySurface() {
    if (mEglSurface != nullptr) {
        mEglSurface->release();
        delete mEglSurface;
    }
}

Painter::~Painter() {

}


