//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_UPLOADER_H
#define PBOTEST_UPLOADER_H

/**
 * upload texture and manage pbo
 */



#include <GLES3/gl3.h>
#include <mutex>
#include <condition_variable>

#include "util/Looper.h"
#include "gl/EglCore.h"
#include "RenderTask.h"
#include "Painter.h"
#include "framesource/DecodeThread.h"
#include "effect/Effect.h"


class Uploader;

typedef struct PboRes {
    GLuint pbo;
    GLsync sync;
    int state;
//    Uploader *uploader;

    bool isReady();
} PboRes;


typedef struct RenderResRequest {
    RenderResRequest(RenderTask *task, int resIndex);

    RenderTask *task;
    int resIndex;
} RenderResRequest;

class UploadReq;

class Uploader : public Looper {
public:


    virtual ~Uploader();

    Uploader(EGLContext *sharedContext, Painter *painter, DecodeThread *decodeThread,
             size_t pboLen = 8);

    void handle(int what, void *data) override;

    PboRes *uploadFrame(void *buf, size_t size);

    void releaseBuffer(PboRes *pboRes);

    void dataFillSuccess(RenderResRequest *pRequest);

    void dataFillFail(RenderResRequest *pRequest);

    void quit() override;

private:
    EglCore *mEglCore;

    PboRes *mPboResArray;
    int mArrayLen;
    Painter *mPainter;
    DecodeThread *mDecodeThread;
    std::mutex mUploadMutex;
    std::condition_variable mGetPboCv;

    std::queue<UploadReq *> mPendingReqs;

    void startUploader(EGLContext *sharedContext);

    void requestRenderRes(RenderTask *pTask);

    void prepareFrameSource(Effect *pEffect);

    void handleUploadAndGetPboBuf(UploadReq *pReq);

    void handleReleasePboBuf(PboRes *pRes);

    void handleDataFillReady(RenderResRequest *pRequest);

    void handleDataFillFail(RenderResRequest *pRequest);

    void handleDestroyPboBuf();
};


#endif //PBOTEST_UPLOADER_H
