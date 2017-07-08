//
// Created by ranqingguo on 7/7/17.
//

#include "Uploader.h"

enum {
    kWhatStart,
    kWhatUploadBufAndGetPbo,
    kWhatReleasePboBuf,
    kWhatUnmapPboAndCheckDrawFire,
    kWhatRequestRenderRes,
    kWhatDataFillReady,
    kWhatDataFillFail,
    kWhatPrepareFrameSource,
};

enum {
    PboResReady,
    PboResNotUsed,
    PboResInvalid,
    PboResPrepareing
};

typedef struct UploadReq {
    PboRes *pboRes;
    void *buf;
    size_t size;
    bool deal;
} UploadReq;

void Uploader::handle(int what, void *data) {
    switch (what) {
        case kWhatStart:
            startUploader((EGLContext *) data);
            break;
        case kWhatUploadBufAndGetPbo:
            handleUploadAndGetPboBuf((UploadReq *) data);
            break;
        case kWhatReleasePboBuf:
            handleReleasePboBuf((PboRes *) data);
            break;
        case kWhatUnmapPboAndCheckDrawFire:
            break;
        case kWhatRequestRenderRes:
            requestRenderRes((RenderTask *) data);
            break;
        case kWhatDataFillReady:
            break;
        case kWhatDataFillFail:
            break;
        case kWhatPrepareFrameSource:
            prepareFrameSource((Effect *) data);
            break;
        default:
            break;
    }
}

Uploader::~Uploader() {
    quit();

    if (mEglCore != nullptr)
        mEglCore->tearDown();
}

Uploader::Uploader(EGLContext *sharedContext, Painter *painter, DecodeThread *decodeThread)
        : Looper(), mPainter(painter), mDecodeThread(decodeThread) {
    post(kWhatStart, sharedContext);
}

void Uploader::startUploader(EGLContext *sharedContext) {
    mEglCore = new EglCore(sharedContext);
}

void Uploader::requestRenderRes(RenderTask *pTask) {
    int size = -1;
    pTask->getFrameSourceArray(&size);

    for (int i = 0; i < size; ++i) {
        mDecodeThread->requestFrame(new RenderResRequest(pTask, i));
    }
}

void Uploader::prepareFrameSource(Effect *pEffect) {
    int size = -1;
    auto frameArray = pEffect->getFrameSourceArray(&size);

    for (int i = 0; i < size; ++i) {
        if (!frameArray[i].isPrepared()) {
            mDecodeThread->prepareRes(&frameArray[i]);
        }
    }

}

RenderResRequest::RenderResRequest(RenderTask *task, int resIndex) : task(task),
                                                                     resIndex(resIndex) {}

//
//PboRes *Uploader::getBuffer() {
//    std::unique_lock lock(mGetPboMuxtex);
//    GetBufReq getBufReq;
//    getBufReq.pboRes = nullptr;
//    post(kWhatGetPboBuf, &getBufReq);
//
//    mGetPboCv.wait(lock, [getBufReq]() { return getBufReq.pboRes != nullptr; });
//
//    return getBufReq.pboRes;
//}
//
//void Uploader::queueBuffer(RenderResRequest *pRequest) {
//    auto pboRes = pRequest->task->getPboResAt(pRequest->resIndex);
//
//    glUnmapBuffer(pboRes->pbo);
//}


void Uploader::releaseBuffer(PboRes *pboRes) {
    post(kWhatReleasePboBuf, pboRes);
}

void Uploader::handleReleasePboBuf(PboRes *pRes) {
    pRes->state = PboResNotUsed;
    pRes->sync = nullptr;

    if (mPendingReqs.size() > 0) {
        post(kWhatUploadBufAndGetPbo, mPendingReqs.front());
        mPendingReqs.pop();
    }
}

void Uploader::handleUploadAndGetPboBuf(UploadReq *pReq) {
    PboRes *pboRes = nullptr;
    for (int i = 0; i < mArrayLen; ++i) {
        if (mPboResArray[i].state == PboResNotUsed) {
            mPboResArray[i].state = PboResPrepareing;
            pboRes = &mPboResArray[i];

            break;
        }
    }

    if (pboRes == nullptr) {
        mPendingReqs.push(pReq);
        return;
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboRes->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr) pReq->size, pReq->buf, GL_STREAM_DRAW);
    pboRes->state = PboResReady;
    pReq->pboRes = pboRes;
    mGetPboCv.notify_all();
}

PboRes *Uploader::uploadFrame(void *buf, size_t size) {
    UploadReq req;

    req.pboRes = nullptr;
    req.buf = buf;
    req.size = size;
    req.deal = false;

    std::unique_lock<std::mutex> lock(mUploadMutex);
    post(kWhatUploadBufAndGetPbo, &req);
    mGetPboCv.wait(lock, [req]() { return req.deal; });

    return req.pboRes;
}

bool PboRes::isReady() {
    return state == PboResReady;
}
