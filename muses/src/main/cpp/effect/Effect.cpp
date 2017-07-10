//
// Created by ranqingguo on 7/7/17.
//

#include "Effect.h"


long Effect::getStartTimeUs() const {
    return mStartTimeUs;
}

long Effect::getEndTimeUs() const {
    return mEndTimeUs;
}

Effect::Effect(long startTimeUs, long endTimeUs) : mStartTimeUs(
        startTimeUs), mEndTimeUs(endTimeUs) {


}

Effect::Effect(long durationUs) {
    mStartTimeUs = 0;
}

bool Effect::offsetTime(long offsetUs) {
    if (mStartTimeUs + offsetUs < 0)
        return false;

    mStartTimeUs += offsetUs;
    mEndTimeUs += offsetUs;
    return true;
}


std::vector<std::shared_ptr<IFrameSource>> Effect::getFrameSourceVector() {
    return mFrameSourceVector;
}

Effect::~Effect() {

}

std::vector<std::shared_ptr<Render>> Effect::getRenderVector() {
    return mRenderVector;
}


