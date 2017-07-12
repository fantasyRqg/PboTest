//
// Created by ranqingguo on 7/10/17.
//

#include "TestEffect.h"
#include "../render/PolygonOffsetRenderer.h"
#include "../framesource/VideoFrameSource.h"

TestEffect::TestEffect(long durationUs) : Effect(durationUs) {

    std::shared_ptr<Render> p(new PolygonOffsetRenderer());
    mRenderVector.push_back(p);

//    std::shared_ptr<VideoFrameSource> v1(
//            new VideoFrameSource("/sdcard/DCIM/Hover_Camera/100EJEGQ_VID_0041_47a0bba.mp4"));
//    std::shared_ptr<VideoFrameSource> v2(
//            new VideoFrameSource("/sdcard/DCIM/Hover_Camera/100EJEGQ_VID_0076_ff0fa2b.mp4"));

    std::shared_ptr<VideoFrameSource> v1(
            new VideoFrameSource("/sdcard/DCIM/Hover_Camera/100APCPP_VID_0313_1e53b82.mp4"));
    std::shared_ptr<VideoFrameSource> v2(
            new VideoFrameSource("/sdcard/DCIM/Hover_Camera/100APCPP_VID_0273_a4260b5.mp4"));

    mFrameSourceVector.push_back(v1);
    mFrameSourceVector.push_back(v2);

}

RenderTask *TestEffect::nextRenderTask() {
    mRunUs += 33333;
    return new RenderTask(mStartTimeUs + mRunUs, mFrameSourceVector,
                          mRenderVector);
}