//
// Created by ranqingguo on 7/7/17.
//
#include <android/asset_manager_jni.h>

#include "Painter.h"
#include "gl/surface/WindowSurface.h"
#include "util/common.h"
#include "Player.h"

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
        "kWhatNewPlay",
};


Painter::Painter(AAssetManager *assetManager, JavaVM *javaVM) :
        Looper("Painter"),
        mAssetManager(assetManager), mJavaVm(javaVM) {
    post(kWhatStart, nullptr);
}

void Painter::loop() {

    auto result = mJavaVm->AttachCurrentThread(&mEnv, NULL);

    LOGI("Painter attach jvm %s", result == JNI_OK ? "success" : "failure");

    Looper::loop();
    if (mJavaVm != nullptr && mEnv != nullptr) {
        mJavaVm->DetachCurrentThread();
//        mJavaVm->DestroyJavaVM();
        mEnv = nullptr;
    }
}

void Painter::postDrawRenderTask(RenderTask *task) {
    post(kWhatDrawRenderTask, task);
}

void Painter::handle(int what, void *data) {
    LOGI("%s what = %s ", mName.c_str(), painterStr[what].c_str());

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

    pTask->updateImage(mEnv);
    mPlayer->requestNextFrame();

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

    mPlayer->postTaskFinished(pTask);

}


void Painter::handleStart() {
    mEglCore = new EglCore;
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
    for (auto fs: pEffect->getFrameSourceVector()) {
        fs->unbindTexture(mEnv);
    }
    for (auto r : pEffect->getRenderVector()) {
        r->tearDown();
    }
}

void Painter::handleRenderSetup(Effect *pEffect) {
    int texCount = 0;
    auto fsv = pEffect->getFrameSourceVector();
    for (auto r: pEffect->getRenderVector()) {
        r->setUp(mAssetManager, mEglSurface);

        for (int i = 0; i < r->getTextureCount(); ++i) {
            fsv[texCount + i]->bindTexture(mEnv, r->getTextureIdAt(i), nullptr);
        }
    }

    mDecodeTread->prepareEffect(pEffect);

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

void Painter::bindComponent(Player *player, DecodeThread *decodeThread) {
    mPlayer = player;
    mDecodeTread = decodeThread;
}





