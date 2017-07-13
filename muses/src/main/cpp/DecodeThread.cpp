//
// Created by ranqingguo on 7/7/17.
//

#include <sstream>
#include "DecodeThread.h"
#include "Player.h"
#include "util/common.h"

#undef TAG
#define TAG "DecodeThread"

enum {
    kWhatQueueInputBuffer,
    kWhatSeek,
    kWhatReleaseRes,
    kWhatPrepareRes,
    kWhatRequestFrame,
    kWhatSkipFrame,
    kWhatFrameAvailable,
    kWhatReleaseEffect,
    kWhatPrepareEffect,
    kWhatRequestResFail,
};


std::string DecodeStr[]{
        "kWhatQueueInputBuffer",
        "kWhatSeek",
        "kWhatReleaseRes",
        "kWhatPrepareRes",
        "kWhatRequestFrame",
        "kWhatSkipFrame",
        "kWhatFrameAvailable",
        "kWhatReleaseEffect",
        "kWhatPrepareEffect",
        "kWhatRequestResFail",
};

typedef struct SkipReq {
    RenderResRequest *rrr;
    int skip;
} SkipReq;


void DecodeThread::handle(int what, void *data) {
    LOGD("%s what = %s ", mName.c_str(), DecodeStr[what].c_str());
    switch (what) {
        case kWhatPrepareRes: {
            auto ifs = ((IFrameSource *) data);
            if (!ifs->prepare(this)) {
                std::stringstream ss;
                ss << "prepare res error ,name = [" << ifs->getName() << "]";
                mPlayer->postOnError(new std::runtime_error(ss.str()));
            }

        }
            break;

        case kWhatQueueInputBuffer: {
            auto pVfs = (VideoFrameSource *) data;
            auto index = ((VideoFrameSource *) data)->queueInputBuffer();
//            if (index >= 0) {
//                post(kWhatQueueInputBuffer, pVfs);
//            }
//            LOGD("kWhatQueueInputBuffer  %s    %d", pVfs->getName().c_str(), index);
        }
            break;

        case kWhatSeek:
            break;

        case kWhatRequestFrame:
            handleRequestFrame((RenderResRequest *) data);
            break;

        case kWhatReleaseRes: {
            ((IFrameSource *) data)->release();
        }
            break;

        case kWhatSkipFrame:
            handleSkipFrame((SkipReq *) data);
            break;

        case kWhatFrameAvailable:
            handleFrameAvailable((RenderResRequest *) data);
            break;

        case kWhatReleaseEffect:
            handleReleaseEffect((Effect *) data);
            break;

        case kWhatPrepareEffect:
            handlePrepareEffect((Effect *) data);
            break;

        case kWhatRequestResFail:
            handleRequestResFail((RenderResRequest *) data);
            break;
        default:
            break;
    }

}

DecodeThread::~DecodeThread() {
    quit();
}

DecodeThread::DecodeThread() : Looper("DecodeThread") {}

void DecodeThread::requestFrame(RenderResRequest *rrr) {
    rrr->start = glCommon::systemnanotime();
    post(kWhatRequestFrame, rrr);

}

void DecodeThread::handleRequestFrame(RenderResRequest *pRequest) {
//    auto start = glCommon::systemnanotime();
//    LOGV("in handleRequestFrame index %lld , take %lld", pRequest->task->getPresentTimeUs(),
//         glCommon::systemnanotime() - pRequest->start);
    auto pFs = pRequest->task->getFrameSourceAt(pRequest->resIndex);
    auto skip = pRequest->task->getSkipFrameAt(pRequest->resIndex);

    if (skip > 0) {
        auto sr = new SkipReq();
        sr->rrr = pRequest;
        sr->skip = skip;
        post(kWhatSkipFrame, &sr);
        return;
    }

    if (!pFs->isPrepared()) {
        pRequest->task->setFrameReady(false, pRequest->resIndex);
        requestResFail(pRequest);
        return;
    }

    auto ok = pFs->requestFrame(this, pRequest);

    if (!ok) {
        requestFrame(pRequest);
    }
}

//void DecodeThread::prepareRes(IFrameSource *pSource) {
//    post(kWhatPrepareRes, pSource);
//}

void DecodeThread::queueInputBuffer(VideoFrameSource *pVfs) {
    post(kWhatQueueInputBuffer, pVfs);
}


void DecodeThread::handleSkipFrame(SkipReq *pReq) {
    auto rrr = pReq->rrr;
    auto fs = rrr->task->getFrameSourceAt(rrr->resIndex);
    if (fs->isPrepared() && fs->skipOneFrame()) {
        pReq->skip--;
        if (pReq->skip > 0) {
            post(kWhatSkipFrame, pReq);
        } else {
            requestFrame(pReq->rrr);
            delete pReq;
        }
    }
}


void DecodeThread::bindComponent(Painter *painter, Player *player) {
    mPlayer = player;
    mPainter = painter;
}

void DecodeThread::onFrameAvailable(RenderResRequest *pRequest) {
    post(kWhatFrameAvailable, pRequest);
}

void DecodeThread::handleFrameAvailable(RenderResRequest *pRequest) {
    LOGI("handleFrameAvailable name = %lld take = %lld", pRequest->task->getPresentTimeUs(),
         glCommon::systemnanotime() - pRequest->start);
    auto task = pRequest->task;
    task->setFrameReady(true, pRequest->resIndex);
    if (task->isFramePrepared()) {
        mPainter->postDrawRenderTask(task);
    }

    delete pRequest;

    if (task->isAllFrameProcessed() && !task->isFramePrepared()) {
        mPlayer->renderTaskFail(task);
    }
}

void DecodeThread::requestResFail(RenderResRequest *pRequest) {
    post(kWhatRequestResFail, pRequest);
}

void DecodeThread::handleRequestResFail(RenderResRequest *pRequest) {
    LOGW("handleRequestResFail take = %lld", glCommon::systemnanotime() - pRequest->start);

    auto task = pRequest->task;
    task->setFrameReady(false, pRequest->resIndex);

    if (task->isAllFrameProcessed()) {
        mPlayer->renderTaskFail(task);
    }
}

void DecodeThread::releaseEffect(Effect *pEffect) {
    post(kWhatReleaseEffect, pEffect);
}

void DecodeThread::handleReleaseEffect(Effect *pEffect) {
    auto fsv = pEffect->getFrameSourceVector();

    for (auto fs:fsv) {
        fs->release();
    }

    mPainter->tearDownRender(pEffect);
}

void DecodeThread::postRenderTask(RenderTask *pTask) {
    int size = pTask->getFrameSourceVector().size();

    if (size > 0) {
        for (int i = 0; i < size; ++i) {
            requestFrame(new RenderResRequest(pTask, i, this));
        }
    } else {
        mPainter->postDrawRenderTask(pTask);
    }
}


void DecodeThread::prepareEffect(Effect *pEffect) {
    post(kWhatPrepareEffect, pEffect);
}

void DecodeThread::handlePrepareEffect(Effect *pEffect) {
    for (auto fs:pEffect->getFrameSourceVector()) {
        if (!fs->prepare(this)) {
            std::stringstream ss;
            ss << "prepare frame source " << fs->getName() << " fail";
            mPlayer->postOnError(new std::runtime_error(ss.str()));;
            return;
        }
    }

    mPlayer->postEffectPrepared(pEffect);
}


RenderResRequest::RenderResRequest(RenderTask *task, int resIndex, DecodeThread *thread)
        : task(task), resIndex(resIndex), decodeThread(thread) {

}
