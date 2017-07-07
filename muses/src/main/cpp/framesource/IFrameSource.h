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
    /**
     * specify start and end time
     * @param startMills
     * @param endMills
     */
    IFrameSource(long startMills, long endMills);


    virtual bool prepare() = 0;

    virtual bool requestFrame() = 0;

    virtual bool isOk() = 0;

    void setCallback(const FrameReadyCallback &callback);

    long getStartUs() const;

    long getEndUs() const;
//
//    /**
//     * first frame on effect timeLine
//     * @return
//     */
//    long getFrameStartMills() const;
//
//    void setFrameStartMills(long frameStartMills);
//
//    /**
//     * play start = start -duration
//     * @param durationMills
//     * @return
//     */
//    bool appendStartMills(long durationMills);
//
//    /**
//     * play end = end - duration
//     * @param durationMills
//     * @return
//     */
//    bool appendEndMills(long durationMills);



protected:
    long mStartUs = -1;
    long mEndUs = -1;

//    long mFrameStartMills;
//
//    long mFirstFrameUs;
//    /**
//     * some time effect will use more frame,so adjust duration
//     */
//    long mPlayStartMills;
//
//    /**
//     * some time effect will use more frame,so adjust duration
//     */
//    long mPlayEndMills;

    FrameReadyCallback mCallback;
};

#endif //MUSES_IFRAMESOURCE_H
