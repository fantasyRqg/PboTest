//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_DECODETHREAD_H
#define PBOTEST_DECODETHREAD_H


#include <thread>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaCodec.h>

#include "VideoFrameSource.h"
#include "IFrameSource.h"
#include "../util/Looper.h"

class Uploader;

class RenderTask;

struct RenderResRequest;

struct SkipReq;

class DecodeThread : public Looper {
public:


    DecodeThread();

    virtual ~DecodeThread();


    void prepareRes(IFrameSource *pSource);

    void requestFrame(RenderResRequest *pRequest);

    void queueInputBuffer(VideoFrameSource *pVfs);

    void bindUploader(Uploader *uploader);

    void releaseRes(IFrameSource *pSource);

private:
    void handle(int what, void *data) override;

    Uploader *mUploader;


    void handleRequestFrame(RenderResRequest *pRequest);

    void handleSkipFrame(SkipReq *pReq);
};


#endif //PBOTEST_DECODETHREAD_H
