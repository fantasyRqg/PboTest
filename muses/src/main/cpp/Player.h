//
// Created by ranqingguo on 7/10/17.
//

#ifndef PBOTEST_PLAYER_H
#define PBOTEST_PLAYER_H

#include <memory>

#include "util/Looper.h"
#include "EffectManager.h"
#include "DecodeThread.h"
#include "Painter.h"


class Painter;


class Player : public Looper {
public:
    Player(DecodeThread *decodeThread, Painter *painter);

    void play(EffectManager *pEm);

    void stop();

    void pause();

    void resume();

    void seekTo(long timeUs);

    void replay();

    void speedChange(float x);

    void requestNextFrame();

    void postOnError(std::runtime_error *pError);


    void renderTaskFail(RenderTask *pTask);

    void postTaskFinished(RenderTask *pTask);

private:
    struct SeekReq;
    struct SpeedReq;

    DecodeThread *mDecodeThread;
    Painter *mPainter;

    EffectManager *mCurrentPlay;
    bool mPlayRun = false;

    void handle(int what, void *data) override;

    void handlePlay(EffectManager *pManager);

    void handleStopCurrent();

    void handlePause();

    void handleResume();

    void handleSeek(SeekReq *pReq);

    void handleReplay();

    void handleSpeedChange(SpeedReq *pReq);

    void handleNextFrame();

    void playOneFrame(EffectManager *pManager);

    void advanceEffectManager(EffectManager *pManager, Effect *effect);

    void handleRenderTaskFail(RenderTask *pTask);

    void handleTaskFinished(RenderTask *pTask);
};


#endif //PBOTEST_PLAYER_H
