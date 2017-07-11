//
// Created by ranqingguo on 7/10/17.
//

#include "Player.h"
#include "util/common.h"
#include "Uploader.h"

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

std::string PlayerEnumStr[]{
        "kWhatPlay",
        "kWhatStopCurrent",
        "kWhatPause",
        "kWhatResume",
        "kWhatSeek",
        "kWhatReplay",
        "kWhatSpeedChange",
        "kWhatNextFrame",
        "kWhatOnError",
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
//    LOGV("handle what = %s", PlayerEnumStr[what].c_str());
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
    mPlayRun = true;
    mUploader->postNewPlay();
    playOneFrame(mCurrentPlay);
}


void Player::handleStopCurrent() {
    mPlayRun = false;
}

void Player::handlePause() {
    mPlayRun = false;
}

void Player::handleResume() {
    if (mCurrentPlay != nullptr) {
        mPlayRun = true;
        playOneFrame(mCurrentPlay);
    }
}

void Player::handleSeek(SeekReq *pReq) {

}

void Player::handleReplay() {
    auto curr = mCurrentPlay;
    if (curr != nullptr) {
        post(kWhatStopCurrent, nullptr);
        curr->reset();
        post(kWhatPlay, curr);
    }
}

void Player::handleSpeedChange(SpeedReq *pReq) {
    if (mCurrentPlay != nullptr) {
        mPlayRun = true;
        playOneFrame(mCurrentPlay);
    }
}

void Player::handleNextFrame() {
    playOneFrame(mCurrentPlay);
}

void Player::playOneFrame(EffectManager *pManager) {
    auto effect = pManager->getEffect();

    if (effect != nullptr && effect->hasNextFrame()) {
        if (!effect->isPrepared()) {
            mUploader->prepareEffect(effect);
            effect->setPrepared(true);
        }

        RenderTask *task = nullptr;
        try {
            task = effect->nextRenderTask();
        } catch (std::runtime_error e) {
            postOnError(new std::runtime_error(e.what()));
            if (effect->isPrepared()) {
                mUploader->releaseEffect(effect);
            }
            return;
        }

        if (task != nullptr) {
            mUploader->postRenderTask(task);
        } else {
            advanceEffectManager(pManager, effect);
        }
    } else {
        advanceEffectManager(pManager, effect);
    }
}

void Player::advanceEffectManager(EffectManager *pManager, Effect *effect) {
    if (effect != nullptr) {
        mUploader->releaseEffect(effect);
        effect->setPrepared(false);
    }

    if (pManager->advance()) {
        requestNextFrame();
    }
}

void Player::bindUploader(Uploader *uploader) {
    mUploader = uploader;
}

Player::Player() : Looper("Player") {

}




