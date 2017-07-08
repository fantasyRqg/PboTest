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

RenderTask *Effect::nextRenderTask() {

    return nullptr;
}

IFrameSource *Effect::getFrameSourceArray(int *out_size) {
    *out_size = mFSArrayLen;
    return mFrameSourcArray;
}
