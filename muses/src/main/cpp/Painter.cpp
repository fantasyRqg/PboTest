//
// Created by ranqingguo on 7/7/17.
//
#include <android/asset_manager_jni.h>

#include "Painter.h"
#include "Uploader.h"
#include "gl/surface/WindowSurface.h"
#include "util/common.h"

#undef TAG
#define TAG "Painter"

enum {
    kWhatStart,
    kWhatStopCurrent,
    kWhatDrawRenderTask,
    kWhatCreateWindowSurface,
    kWhatCreateOffScreenSurface,
    kWhatDestroySurface,
    kWhatRenderSetup,
    kWhatRenderTearDown,
    kWhatNewPlay,
};

std::string painterStr[]{
        "kWhatStart",
        "kWhatStopCurrent",
        "kWhatDrawRenderTask",
        "kWhatCreateWindowSurface",
        "kWhatCreateOffScreenSurface",
        "kWhatDestroySurface",
        "kWhatRenderSetup",
        "kWhatRenderTearDown",
};


Painter::Painter(AAssetManager *assetManager) : Looper("Painter"), mAssetManager(assetManager) {

}


void Painter::postDrawRenderTask(RenderTask *task) {
    post(kWhatDrawRenderTask, task);
}

void Painter::handle(int what, void *data) {
//    LOGI("%s what = %s ", mName.c_str(), painterStr[what].c_str());

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

        case kWhatRenderSetup:
            handleRenderSetup((Effect *) data);
            break;
        case kWhatRenderTearDown:
            handleRenderTearDown((Effect *) data);
            break;
        case kWhatNewPlay:
            handleNewPlay();
            break;
        default:
            break;
    }
}

int64_t last = glCommon::systemnanotime();

void Painter::handleDrawRenderTask(RenderTask *pTask) {
    LOGD("handleDrawRenderTask diff = %lld", glCommon::systemnanotime() - last);
    last = glCommon::systemnanotime();

    pTask->draw();

    if (mFirstDisplayUs <= 0) {
        mFirstDisplayUs = glCommon::systemnanotime() / 1000L;
    }

    auto pt = pTask->getPresentTimeUs();
    auto curr = glCommon::systemnanotime() / 1000L;
    auto sleepUs = pt - (curr - mFirstDisplayUs);

//    LOGD("sleep = %10lld pt = %10lld curr = %10lld first = %10lld",
//         sleepUs,
//         pt,
//         curr,
//         mFirstDisplayUs);
    if (sleepUs > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(sleepUs));
    }
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

    mEglCore->makeCurrent(mEglSurface->getEglSurface());


    ANativeWindow_Buffer buffer;
    ARect rect;
    ANativeWindow_lock(pWindow, &buffer, &rect);

    ANativeWindow_release(pWindow);

    const EGLint attrs[] =
            {
                    EGL_IMAGE_PRESERVED_KHR,
                    EGL_TRUE,
                    EGL_NONE,
                    EGL_NONE
            };

    auto pEGLImage = eglCreateImageKHR(eglGetCurrentDisplay(),
                                       eglGetCurrentContext(),
                                       EGL_NATIVE_BUFFER_ANDROID,
                                       (EGLClientBuffer) &buffer,
                                       attrs);
    if (pEGLImage == EGL_NO_IMAGE_KHR) {
        LOGI("Error: eglCreateImage() failed at %s:%in", __FILE__, __LINE__);
    }


}

void Painter::handleCreateOffScreenSurface() {

}

void Painter::handleDestroySurface() {
    LOGD("handleDestroySurface");
    if (mEglSurface != nullptr) {
        mEglSurface->release();
        delete mEglSurface;
    }
}

Painter::~Painter() {

}


void Painter::tearDownRender(Effect *pEffect) {
    post(kWhatRenderTearDown, pEffect);
}

void Painter::setUpRender(Effect *pEffect) {
    post(kWhatRenderSetup, pEffect);
}

void Painter::handleRenderTearDown(Effect *pEffect) {
    for (auto r : pEffect->getRenderVector()) {
        r->tearDown();
    }
}

void Painter::handleRenderSetup(Effect *pEffect) {
    for (auto r: pEffect->getRenderVector()) {
        r->setUp(mAssetManager, mEglSurface);
    }
}

void Painter::postCreateWindowSurface(ANativeWindow *pWindow) {
    post(kWhatCreateWindowSurface, pWindow);
}

void Painter::handleNewPlay() {
    mFirstDisplayUs = -1;
    LOGI("first = %lld", mFirstDisplayUs);
}

void Painter::postNewPlay() {
    post(kWhatNewPlay, nullptr);
}





