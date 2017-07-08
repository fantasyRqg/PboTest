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
    GLsync *sync;
    int state;
    Uploader *uploader;

    bool isReady();
} PboRes;


class UploadReq;

class Uploader : public Looper {
public:


    virtual ~Uploader();

    Uploader(EGLContext *sharedContext, Painter *painter, DecodeThread *decodeThread);

    void handle(int what, void *data) override;

    PboRes *uploadFrame(void *buf, size_t size);

    void releaseBuffer(PboRes *pboRes);

private:
    EglCore *mEglCore;

    PboRes *mPboResArray;
    int mArrayLen;
    Painter *mPainter;
    DecodeThread *mDecodeThread;
    std::mutex mUploadMutex;
    std::condition_variable mGetPboCv;

    void startUploader(EGLContext *sharedContext);

    void requestRenderRes(RenderTask *pTask);

    void prepareFrameSource(Effect *pEffect);

    void handleUploadAndGetPboBuf(UploadReq *pReq);

    void handleQueuePboBuf(RenderResRequest *pRequest);

    void handleReleasePboBuf(PboRes *pRes);
};


#endif //PBOTEST_UPLOADER_H
