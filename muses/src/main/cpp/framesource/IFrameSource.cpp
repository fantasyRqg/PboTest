//
// Created by ranqingguo on 7/4/17.
//

#include "IFrameSource.h"


IFrameSource::IFrameSource(long startMills, long endMills) :
        mStartMills(startMills),
        mEndMills(endMills) {

}

void IFrameSource::setCallback(const FrameReadyCallback &callback) {
    mCallback = callback;
}

long IFrameSource::getStartMills() const {
    return mStartMills;
}

long IFrameSource::getEndMills() const {
    return mEndMills;
}
