//
// Created by ranqingguo on 7/8/17.
//

#ifndef PBOTEST_RENDERTASK_H
#define PBOTEST_RENDERTASK_H


#include "framesource/IFrameSource.h"
#include "render/Render.h"

class RenderTask {
public:
    RenderTask(long presentTimeUs, IFrameSource **frameSourceArray, int FSArrayLen,
               Render **renderArray, int renderArrayLen);

    void draw();

//    void prepareDrawFrame();
//
//    void setDrawPboRes();

    bool isTaskValid();

    IFrameSource *getFrameSourceAt(int index);

    int getSkipFrameAt(int index);

    PboRes *getPboResAt(int index);

    IFrameSource **getFrameSourceArray(int *out_size);

    virtual ~RenderTask();

    void setReadyPboRes(PboRes *pRes, int i);

    bool isAllResProcessed();


private:
    long mPresentTimeUs;
    IFrameSource **mFrameSourceArray;
    bool *mProcessedMark;
    int mFSArrayLen;

    Render **mRenderArray;
    int mRenderArrayLen;


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
