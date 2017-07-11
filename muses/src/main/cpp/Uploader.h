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
    int width;
    int height;
//    GLsync sync;
    int state;
//    Uploader *uploader;

    bool isReady();
} PboRes;


typedef struct RenderResRequest {
    RenderResRequest(RenderTask *task, int resIndex);

    RenderTask *task;
    int resIndex;
    int64_t start;
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

    void postRenderTask(RenderTask *pTask);

    void prepareEffect(Effect *pEffect);

    void releaseEffect(Effect *pEffect);

    void postNewPlay();

private:
    EglCore *mEglCore = nullptr;

    PboRes *mPboResArray = nullptr;
    int mArrayLen;
    Painter *mPainter = nullptr;
    DecodeThread *mDecodeThread = nullptr;
    Player *mPlayer = nullptr;

    std::mutex mUploadMutex;
    std::condition_variable mGetPboCv;

    std::mutex mStartLock;
    std::condition_variable mStartedCv;

    std::queue<UploadReq *> mPendingReqs;

    void handle(int what, void *data) override;

    void handleStartUploader(EGLContext sharedContext);

    void handleRequestRenderRes(RenderTask *pTask);

    void handlePrepareEffect(Effect *pEffect);

    void handleUploadAndGetPboBuf(UploadReq *pReq);

    void handleReleasePboBuf(PboRes *pRes);

    void handleDataFillReady(RenderResRequest *pRequest);

    void handleDataFillFail(RenderResRequest *pRequest);

    void handleDestroyPboBuf();

    void handleReleaseEffect(Effect *pEffect);
};


#endif //PBOTEST_UPLOADER_H
