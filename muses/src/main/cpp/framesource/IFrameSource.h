//
// Created by ranqingguo on 7/4/17.
//

#ifndef MUSES_IFRAMESOURCE_H
#define MUSES_IFRAMESOURCE_H

#include <algorithm>
#include <functional>

typedef std::function<void(void *, int)> FrameReadyCallback;

class IFrameSource {

public:
    IFrameSource(long startMills, long endMills);

    virtual bool prepare() = 0;

    virtual bool requestFrame() = 0;

    void setCallback(const FrameReadyCallback &callback);

    long getStartMills() const;

    long getEndMills() const;

protected:
    long mStartMills;
    long mEndMills;
    FrameReadyCallback mCallback;
};

#endif //MUSES_IFRAMESOURCE_H
