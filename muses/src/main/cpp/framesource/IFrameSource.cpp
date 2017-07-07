//
// Created by ranqingguo on 7/4/17.
//

#include "IFrameSource.h"


IFrameSource::IFrameSource(long startMills, long endMills) :
        mStartUs(startMills),
        mEndUs(endMills) {


}

void IFrameSource::setCallback(const FrameReadyCallback &callback) {
    mCallback = callback;
}

long IFrameSource::getStartUs() const {
    return mStartUs;
}

long IFrameSource::getEndUs() const {
    return mEndUs;
}
//
//long IFrameSource::getFrameStartMills() const {
//    return mFrameStartMills;
//}
//
//void IFrameSource::setFrameStartMills(long frameStartMills) {
//    mFrameStartMills = frameStartMills;
//}
//
//
//bool IFrameSource::appendStartMills(long durationMills) {
//    if (durationMills + mStartUs < 0)
//        return false;
//
//    mPlayStartMills = durationMills + mStartUs;
//}
//
//bool IFrameSource::appendEndMills(long durationMills) {
//    mPlayStartMills = mEndUs + durationMills;
//    return true;
//}

//bool IFrameSource::isOk() {
//    return mPlayStartMills > 0 && mPlayEndMills > 0;
//}



