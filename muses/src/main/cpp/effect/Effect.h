//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_VIDEOEFFECT_H
#define PBOTEST_VIDEOEFFECT_H

#include "../RenderTask.h"
#include "../render/Render.h"

/**
 * 描述需要的素材情况，并组装相应的绘制工具
 *
 * describe framesource info which use to draw frame
 * and assemble draw classes
 */
class Effect {
public:


    Effect(long startTimeUs, long endTimeUs);


    /**
     * start time == duration
     * end time = durationUs
     * @param durationUs
     */
    Effect(long durationUs);

    long getStartTimeUs() const;

    long getEndTimeUs() const;

    bool offsetTime(long offsetUs);

    RenderTask *nextRenderTask();

    IFrameSource *getFrameSourceArray(int *out_size);

private:
    long mStartTimeUs;
    long mEndTimeUs;

    IFrameSource *mFrameSourcArray;
    int mFSArrayLen;

    Render *mRenderArray;
    int mRenderArrayLen;

};


#endif //PBOTEST_VIDEOEFFECT_H
