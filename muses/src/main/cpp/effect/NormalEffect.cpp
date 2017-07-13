//
// Created by ranqingguo on 7/13/17.
//

#include "NormalEffect.h"
#include "../framesource/VideoFrameSource.h"
#include "../render/NormalRender.h"

NormalEffect::NormalEffect(std::string videoPath, long durationUs) : Effect(durationUs) {
    std::shared_ptr<IFrameSource> fs(new VideoFrameSource(videoPath));
    mFrameSourceVector.push_back(fs);

    std::shared_ptr<Render> render(new NormalRender());

    mRenderVector.push_back(render);
}


RenderTask *NormalEffect::nextRenderTask() {
    mRunUs += 33333;

    return new RenderTask(mStartTimeUs + mRunUs, mFrameSourceVector, mRenderVector);
}
