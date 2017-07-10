//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_VIDEOEFFECT_H
#define PBOTEST_VIDEOEFFECT_H

#include <memory>

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

    virtual ~Effect();

    long getStartTimeUs() const;

    long getEndTimeUs() const;

    bool offsetTime(long offsetUs);

    virtual RenderTask *nextRenderTask() = 0;

    std::vector<std::shared_ptr<IFrameSource>> getFrameSourceVector();

    std::vector<std::shared_ptr<Render>> getRenderVector();

protected:
    long mStartTimeUs;
    long mEndTimeUs;

    std::vector<std::shared_ptr<IFrameSource>> mFrameSourceVector;

    std::vector<std::shared_ptr<Render>> mRenderVector;

};


#endif //PBOTEST_VIDEOEFFECT_H
