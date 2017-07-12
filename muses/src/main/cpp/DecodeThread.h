//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_DECODETHREAD_H
#define PBOTEST_DECODETHREAD_H


#include <thread>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaCodec.h>

#include "framesource/VideoFrameSource.h"
#include "framesource/IFrameSource.h"
#include "util/Looper.h"
#include "effect/Effect.h"


class Player;

class Painter;

class RenderTask;

class DecodeThread;

struct SkipReq;

typedef struct RenderResRequest {
    RenderResRequest(RenderTask *task, int resIndex, DecodeThread *thread);

    RenderTask *task;
    int resIndex;
    int64_t start;
    DecodeThread *decodeThread;
} RenderResRequest;


class DecodeThread : public Looper {
public:


    DecodeThread();

    virtual ~DecodeThread();


//    void prepareRes(IFrameSource *pSource);

    void requestFrame(RenderResRequest *pRequest);

    void queueInputBuffer(VideoFrameSource *pVfs);

    void bindComponent(Painter *painter, Player *player);

    void onFrameAvailable(RenderResRequest *pRequest);

    void releaseEffect(Effect *pEffect);

    void postRenderTask(RenderTask *pTask);

    void prepareEffect(Effect *pEffect);

private:
    Painter *mPainter;
    Player *mPlayer;

    void handle(int what, void *data) override;

    void handleRequestFrame(RenderResRequest *pRequest);

    void handleSkipFrame(SkipReq *pReq);

    void handleFrameAvailable(RenderResRequest *pRequest);

    void handleReleaseEffect(Effect *pEffect);

    void requestResFail(RenderResRequest *pRequest);

    void handleRequestResFail(RenderResRequest *pRequest);

    void handlePrepareEffect(Effect *pEffect);
};


#endif //PBOTEST_DECODETHREAD_H
