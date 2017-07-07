//
// Created by ranqingguo on 7/7/17.
//

#include <algorithm>
#include "EffectManager.h"



EffectManager::~EffectManager() {
    delete pBaseEffectLine;

}

int EffectManager::getOutFrameRate() const {
    return mOutFrameRate;
}

EffectManager::EffectManager(int outFrameRate, EffectLine *pBaseEffectLine) : mOutFrameRate(
        outFrameRate), pBaseEffectLine(pBaseEffectLine) {}

