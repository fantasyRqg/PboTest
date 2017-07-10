//
// Created by ranqingguo on 7/10/17.
//

#include "Player.h"
#include "util/common.h"

#undef TAG
#define TAG "Player"

enum {
    kWhatPlay,
    kWhatStopCurrent,
    kWhatPause,
    kWhatResume,
    kWhatSeek,
    kWhatReplay,
    kWhatSpeedChange,
    kWhatNextFrame,
    kWhatOnError,
};

typedef struct Player::SeekReq {
    long timeUs;
} SeekReq;

typedef struct Player::SpeedReq {
    float speed;
} SpeedReq;

void Player::play(EffectManager *pEm) {
    post(kWhatStopCurrent, nullptr);
    post(kWhatPlay, pEm);
}


void Player::stop() {
    post(kWhatStopCurrent, nullptr);
}

void Player::pause() {
    post(kWhatPause, nullptr);
}

void Player::resume() {
    post(kWhatResume, nullptr);
}

void Player::seekTo(long timeUs) {
    auto sr = new SeekReq();
    sr->timeUs = timeUs;
    post(kWhatSeek, sr);
}

void Player::replay() {
    post(kWhatReplay, nullptr);
}

void Player::speedChange(float x) {
    auto sr = new SpeedReq();
    sr->speed = x;

    post(kWhatSpeedChange, sr);
}

void Player::requestNextFrame() {
    post(kWhatNextFrame, nullptr);
}

void Player::postOnError(std::runtime_error *pError) {
    post(kWhatOnError, pError);
}


void Player::handle(int what, void *data) {
    switch (what) {
        case kWhatPlay:
            handlePlay((EffectManager *) data);
            break;
        case kWhatStopCurrent:
            handleStopCurrent();
            break;
        case kWhatPause:
            handlePause();
            break;
        case kWhatResume:
            handleResume();
            break;
        case kWhatSeek:
            handleSeek((SeekReq *) data);
            break;
        case kWhatReplay:
            handleReplay();
            break;
        case kWhatSpeedChange:
            handleSpeedChange((SpeedReq *) data);
            break;
        case kWhatNextFrame:
            handleNextFrame();
            break;
        case kWhatOnError: {
            auto err = ((std::runtime_error *) data);
            LOGE("player error : %s", err->what());
            delete err;
            stop();
        }
            break;

        default:
            break;
    }
}

void Player::handlePlay(EffectManager *pManager) {
    mCurrentPlay = pManager;

    playOneFrame(mCurrentPlay);
}


void Player::handleStopCurrent() {

}

void Player::handlePause() {

}

void Player::handleResume() {

}

void Player::handleSeek(SeekReq *pReq) {

}

void Player::handleReplay() {

}

void Player::handleSpeedChange(SpeedReq *pReq) {

}

void Player::handleNextFrame() {

}

void Player::playOneFrame(EffectManager *pManager) {

}




