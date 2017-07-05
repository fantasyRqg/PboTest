//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_IFRAMESOURCE_H
#define PBOTEST_IFRAMESOURCE_H

#include <algorithm>
#include <functional>

typedef std::function<void(void *, int)> FrameReadyCallback;

class IFrameSource {

public:
    IFrameSource(long startMills, long endMills, FrameReadyCallback &&callback);

    virtual bool prepare() = 0;

    virtual bool requestFrame() = 0;

protected:
    long mStartMills;
    long mEndMills;
    FrameReadyCallback mCallback;
};

#endif //PBOTEST_IFRAMESOURCE_H
