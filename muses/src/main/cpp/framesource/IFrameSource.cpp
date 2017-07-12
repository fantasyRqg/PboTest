//
// Created by ranqingguo on 7/4/17.
//

#include "IFrameSource.h"


IFrameSource::IFrameSource(long startMills, long endMills) :
        mStartUs(startMills),
        mEndUs(endMills) {


}


long IFrameSource::getStartUs() const {
    return mStartUs;
}

long IFrameSource::getEndUs() const {
    return mEndUs;
}

int IFrameSource::getWidth() const {
    return mWidth;
}

int IFrameSource::getHeight() const {
    return mHeight;
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



