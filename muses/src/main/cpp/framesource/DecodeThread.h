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

class DecodeThread : public Looper {
public:


    DecodeThread();

    virtual ~DecodeThread();

    void handle(int what, void *data) override;

    void prepareRes(IFrameSource *pSource);

    void requestFrame(RenderResRequest *pRequest);

    void queueInputBuffer(VideoFrameSource *pVfs);

    void bindUploader(Uploader *uploader);

private:

    Uploader *mUploader;


    void handleRequestFrame(RenderResRequest *pRequest);
};


#endif //PBOTEST_DECODETHREAD_H
