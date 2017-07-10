//
// Created by ranqingguo on 7/7/17.
//

#include "EffectManager.h"


EffectManager::~EffectManager() {
    delete mBaseEffectLine;

}

int EffectManager::getOutFrameRate() const {
    return mOutFrameRate;
}

EffectManager::EffectManager(int outFrameRate, EffectLine *pBaseEffectLine) : mOutFrameRate(
        outFrameRate), mBaseEffectLine(pBaseEffectLine) {

}

bool EffectManager::advance() {
    return mBaseEffectLine->advance();
}

Effect *EffectManager::getEffect() {
    return mBaseEffectLine->getCurrent();
}

void EffectManager::reset() {
    mBaseEffectLine->reset();
}
