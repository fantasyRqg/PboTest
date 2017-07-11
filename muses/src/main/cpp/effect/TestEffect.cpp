//
// Created by ranqingguo on 7/10/17.
//

#include "TestEffect.h"
#include "../render/PolygonOffsetRenderer.h"

TestEffect::TestEffect(long durationUs) : Effect(durationUs) {

    std::shared_ptr<Render> p(new PolygonOffsetRenderer());
    mRenderVector.push_back(p);
}

RenderTask *TestEffect::nextRenderTask() {
    mRunUs += 33333;
    return new RenderTask(mStartTimeUs + mRunUs, mFrameSourceVector,
                          mRenderVector);
}