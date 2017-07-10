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
#include "Player.h"


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

struct UploadReq;

class Uploader : public Looper {
public:


    virtual ~Uploader();

    Uploader(Painter *painter, DecodeThread *decodeThread, Player *player, size_t pboLen = 6);


    PboRes *uploadFrame(void *buf, size_t size);

    void releaseBuffer(PboRes *pboRes);

    void dataFillSuccess(RenderResRequest *pRequest);

    void dataFillFail(RenderResRequest *pRequest);

    void startUploader(EGLContext sharedContext);

    void quit() override;

    void postOnError(std::runtime_error *pError);

private:
    EglCore *mEglCore;

    PboRes *mPboResArray;
    int mArrayLen;
    Painter *mPainter;
    DecodeThread *mDecodeThread;
    Player *mPlayer;

    std::mutex mUploadMutex;
    std::condition_variable mGetPboCv;

    std::queue<UploadReq *> mPendingReqs;

    void handle(int what, void *data) override;

    void handleStartUploader(EGLContext sharedContext);

    void requestRenderRes(RenderTask *pTask);

    void prepareFrameSource(Effect *pEffect);

    void handleUploadAndGetPboBuf(UploadReq *pReq);

    void handleReleasePboBuf(PboRes *pRes);

    void handleDataFillReady(RenderResRequest *pRequest);

    void handleDataFillFail(RenderResRequest *pRequest);

    void handleDestroyPboBuf();
};


#endif //PBOTEST_UPLOADER_H
