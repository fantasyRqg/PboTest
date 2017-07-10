//
// Created by ranqingguo on 7/10/17.
//

#include "TestEffect.h"
#include "../util/common.h"
#include "../render/PolygonOffsetRenderer.h"

TestEffect::TestEffect(long durationUs) : Effect(durationUs) {

    std::shared_ptr<Render> p(new PolygonOffsetRenderer());
    mRenderVector.push_back(p);
}

RenderTask *TestEffect::nextRenderTask() {
    return new RenderTask((long) (glCommon::systemnanotime() / 1000L), mFrameSourceVector,
                          mRenderVector);
}
