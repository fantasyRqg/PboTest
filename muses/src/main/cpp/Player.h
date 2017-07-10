//
// Created by ranqingguo on 7/10/17.
//

#ifndef PBOTEST_PLAYER_H
#define PBOTEST_PLAYER_H

#include <memory>

#include "util/Looper.h"
#include "EffectManager.h"


class Uploader;

class Painter;


class Player : public Looper {
public:
    void play(EffectManager *pEm);

    void stop();

    void pause();

    void resume();

    void seekTo(long timeUs);

    void replay();

    void speedChange(float x);

    void requestNextFrame();

    void postOnError(std::runtime_error *pError);

private:
    struct SeekReq;
    struct SpeedReq;
    EffectManager *mCurrentPlay;

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
};


#endif //PBOTEST_PLAYER_H
