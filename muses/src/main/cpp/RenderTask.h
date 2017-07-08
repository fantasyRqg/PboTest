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

    void prepareDrawFrame();

    void setDrawPboRes();

    bool isTaskValid();

    IFrameSource *getFrameSourceAt(int index);

    int getSkipFrameAt(int index);

    PboRes *getPboResAt(int index);

    IFrameSource **getFrameSourceArray(int *out_size);

    virtual ~RenderTask();

    void setReadyPboRes(PboRes *pRes, int i);

private:
    long mPresentTimeUs;
    IFrameSource **mFrameSourceArray;
    int mFSArrayLen;

    Render **mRenderArray;
    int mRenderArrayLen;


    int *mSkipFrameArray;
    PboRes **mPboResArray;

};


#endif //PBOTEST_RENDERTASK_H
