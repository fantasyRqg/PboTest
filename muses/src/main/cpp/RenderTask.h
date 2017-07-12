//
// Created by ranqingguo on 7/8/17.
//

#ifndef PBOTEST_RENDERTASK_H
#define PBOTEST_RENDERTASK_H


#include "framesource/IFrameSource.h"
#include "render/Render.h"

class RenderTask {
public:

    RenderTask(int64_t presentTimeUs,
               const std::vector<std::shared_ptr<IFrameSource>> &frameSourceVector,
               const std::vector<std::shared_ptr<Render>> &renderVector);

    void draw();

//    void prepareDrawFrame();
//
//    void setDrawPboRes();

    bool isFramePrepared();

    IFrameSource *getFrameSourceAt(int index);

    int getSkipFrameAt(int index);

    std::vector<std::shared_ptr<IFrameSource>> getFrameSourceVector();

    virtual ~RenderTask();

    void setFrameReady(bool ready, int i);

    bool isAllFrameProcessed();

    int64_t getPresentTimeUs() const;


    void updateImage(JNIEnv *pEnv);

private:
    int64_t mPresentTimeUs;
    std::vector<std::shared_ptr<IFrameSource>> mFrameSourceVector;
    int *mFrameMark;

    std::vector<std::shared_ptr<Render>> mRenderVector;

    int *mSkipFrameArray;


//    /**
//     * mark whether skip this render task, use for when error occur on load resources or on draw
//     * mark true to skip this render task
//     */
//    bool mSkipped = false;
//public:
//    bool isSkipped() const;
//
//    void setSkipped(bool skipped);

};


#endif //PBOTEST_RENDERTASK_H
