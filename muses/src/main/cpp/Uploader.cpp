//
// Created by ranqingguo on 7/7/17.
//

#include "Uploader.h"
#include "util/common.h"

#undef TAG
#define TAG "Uploader"

enum {
    kWhatStart,
    kWhatUploadBufAndGetPbo,
    kWhatReleasePboBuf,
    kWhatDrawFire,
    kWhatRequestRenderRes,
    kWhatDataFillReady,
    kWhatDataFillFail,
    kWhatPrepareFrameSource,
    kWhatDestroyPboBuf,
    kWhatOnError,
};

enum {
    PboResReady,
    PboResNotUsed,
    PboResPreparing
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
            handleStartUploader((EGLContext *) data);
            break;
        case kWhatUploadBufAndGetPbo:
            handleUploadAndGetPboBuf((UploadReq *) data);
            break;
        case kWhatReleasePboBuf:
            handleReleasePboBuf((PboRes *) data);
            break;
        case kWhatDrawFire: {
            mPainter->postDrawRenderTask((RenderTask *) data);
            mPlayer->requestNextFrame();
        }
            break;
        case kWhatRequestRenderRes:
            requestRenderRes((RenderTask *) data);
            break;
        case kWhatDataFillReady:
            handleDataFillReady((RenderResRequest *) data);
            break;
        case kWhatDataFillFail:
            handleDataFillFail((RenderResRequest *) data);
            break;
        case kWhatPrepareFrameSource:
            prepareFrameSource((Effect *) data);
            break;
        case kWhatDestroyPboBuf:
            handleDestroyPboBuf();
            break;

        case kWhatOnError: {
            mPlayer->postOnError((std::runtime_error *) data);
        }
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

Uploader::Uploader(Painter *painter, DecodeThread *decodeThread, Player *player, size_t pboLen)
        : Looper(), mPainter(painter), mDecodeThread(decodeThread), mPlayer(player) {

    mPainter->bindUploader(this);
    mDecodeThread->bindUploader(this);

    mArrayLen = pboLen;

}


void Uploader::startUploader(EGLContext sharedContext) {
    post(kWhatStart, sharedContext);
}

void Uploader::handleStartUploader(EGLContext sharedContext) {
    mEglCore = new EglCore(sharedContext);
    try {
        mEglCore->makeCurrent(EGL_NO_DISPLAY);
    } catch (std::runtime_error e) {
        LOGE("make current error : %s", e.what());
        postQuit();
        return;
    }


    mPboResArray = new PboRes[mArrayLen];

    GLuint pboIds[mArrayLen];
    glGenBuffers(mArrayLen, pboIds);

    for (int i = 0; i < mArrayLen; ++i) {
        auto br = mPboResArray[i];

        br.sync = nullptr;
        br.state = PboResNotUsed;
        br.pbo = pboIds[i];
    }
}

void Uploader::requestRenderRes(RenderTask *pTask) {
    int size = pTask->getFrameSourceVector().size();


    for (int i = 0; i < size; ++i) {
        mDecodeThread->requestFrame(new RenderResRequest(pTask, i));
    }
}

void Uploader::prepareFrameSource(Effect *pEffect) {
    auto frames = pEffect->getFrameSourceVector();

    for (auto fa:frames) {
        if (!fa->isPrepared()) {
            mDecodeThread->prepareRes(fa.get());
        }
    }

}

RenderResRequest::RenderResRequest(RenderTask *task, int resIndex) : task(task),
                                                                     resIndex(resIndex) {}

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
            mPboResArray[i].state = PboResPreparing;
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
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    pboRes->state = PboResReady;
    pboRes->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
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

void Uploader::dataFillSuccess(RenderResRequest *pRequest) {
    post(kWhatDataFillReady, pRequest);
}

void Uploader::dataFillFail(RenderResRequest *pRequest) {
    post(kWhatDataFillFail, pRequest);
}

void Uploader::handleDataFillReady(RenderResRequest *pRequest) {
    auto task = pRequest->task;
    if (task->isTaskValid()) {
        post(kWhatDrawFire, task);
    }

    delete pRequest;

    if (task->isAllResProcessed() && !task->isTaskValid()) {
        delete task;
    }

}

void Uploader::handleDataFillFail(RenderResRequest *pRequest) {
    auto task = pRequest->task;
    task->setReadyPboRes(nullptr, pRequest->resIndex);

    if (task->isAllResProcessed()) {
        delete task;
    }
}

void Uploader::handleDestroyPboBuf() {
    GLuint pboIds[mArrayLen];
    for (int i = 0; i < mArrayLen; ++i) {
        pboIds[i] = mPboResArray[i].pbo;
    }

    glDeleteBuffers(mArrayLen, pboIds);
    delete[] mPboResArray;
}

void Uploader::quit() {
    post(kWhatDestroyPboBuf, nullptr);
    Looper::quit();
}

void Uploader::postOnError(std::runtime_error *pError) {
    post(kWhatOnError, pError);
}


bool PboRes::isReady() {
    return state == PboResReady;
}
