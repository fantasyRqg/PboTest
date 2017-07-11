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

    bool isTaskValid();

    IFrameSource *getFrameSourceAt(int index);

    int getSkipFrameAt(int index);

    PboRes *getPboResAt(int index);

    std::vector<std::shared_ptr<IFrameSource>> getFrameSourceVector();

    virtual ~RenderTask();

    void setReadyPboRes(PboRes *pRes, int i);

    bool isAllResProcessed();

    int64_t getPresentTimeUs() const;

private:
    int64_t mPresentTimeUs;
    std::vector<std::shared_ptr<IFrameSource>> mFrameSourceVector;
    bool *mProcessedMark;

    std::vector<std::shared_ptr<Render>> mRenderVector;

    int *mSkipFrameArray;
    PboRes **mPboResArray;

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
