//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_PAINTER_H
#define PBOTEST_PAINTER_H

#include "util/Looper.h"
#include "RenderTask.h"
#include "gl/EglCore.h"
#include "gl/surface/EglSurfaceBase.h"
#include "effect/Effect.h"


class Player;

class DecodeThread;

/**
 * response for draw frame
 */
class Painter : public Looper {
public:
    Painter(AAssetManager *assetManager, JavaVM *javaVM);

    virtual ~Painter();

    void postDrawRenderTask(RenderTask *task);


    void handleDrawRenderTask(RenderTask *pTask);

    void bindComponent(Player *player, DecodeThread *decodeThread);

    void quit() override;

    void tearDownRender(Effect *pEffect);

    void setUpRender(Effect *pEffect);

    void postCreateWindowSurface(ANativeWindow *pWindow);

    void postNewPlay();

private:
    EglCore *mEglCore;
    EglSurfaceBase *mEglSurface;
    AAssetManager *mAssetManager;
    int64_t mFirstDisplayUs;
    DecodeThread *mDecodeTread;
    Player *mPlayer;

    JavaVM *mJavaVm;
    JNIEnv *mEnv;

    void handle(int what, void *data) override;

    void handleStart();

    void handleCreateWindowSurface(NativeWindowType pWindow);

    void handleCreateOffScreenSurface();

    void handleDestroySurface();

    void handleStop();

    void handleRenderSetup(Effect *pEffect);

    void handleRenderTearDown(Effect *pEffect);

    void handleNewPlay();

protected:
    void loop() override;
};


#endif //PBOTEST_PAINTER_H
