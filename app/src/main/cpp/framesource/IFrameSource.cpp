//
// Created by ranqingguo on 7/4/17.
//

#include "IFrameSource.h"


IFrameSource::IFrameSource(long startMills, long endMills, FrameReadyCallback &&callback) :
        mStartMills(startMills),
        mEndMills(endMills),
        mCallback(std::move(callback)) {

}
