//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_IFRAMESOURCE_H
#define PBOTEST_IFRAMESOURCE_H


class IFrameSource {
public:
    IFrameSource(long startMills, long endMills);

    virtual bool prepare() = 0;

    virtual bool requestFrame() = 0;

private:
    long mStartMills;
    long mEndMills;
};

class FrameReadyCallback {
public:
    virtual void onFrameReady(void *frameData) = 0;
};

#endif //PBOTEST_IFRAMESOURCE_H
