//
// Created by ranqingguo on 7/7/17.
//

#include <sstream>
#include "DecodeThread.h"
#include "../RenderTask.h"
#include "../Uploader.h"

#undef TAG
#define TAG "DecodeThread"

enum {
    kWhatQueueInputBuffer,
    kWhatSeek,
    kWhatReleaseRes,
    kWhatPrepareRes,
    kWhatRequestFrame,
    kWhatSkipFrame,
};


std::string DecodeStr[]{
        "kWhatQueueInputBuffer",
        "kWhatSeek",
        "kWhatReleaseRes",
        "kWhatPrepareRes",
        "kWhatRequestFrame",
        "kWhatSkipFrame",
};

typedef struct SkipReq {
    RenderResRequest *rrr;
    int skip;
} SkipReq;


void DecodeThread::handle(int what, void *data) {
//    int64_t last = glCommon::systemnanotime();
    switch (what) {
        case kWhatPrepareRes: {
            auto ifs = ((IFrameSource *) data);
            if (!ifs->prepare(this)) {
                std::stringstream ss;
                ss << "prepare res error ,name = [" << ifs->getName() << "]";
                mUploader->postOnError(new std::runtime_error(ss.str()));
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
        default:
            break;
    }
//    LOGV("%s what = %s currNs = %lld", mName.c_str(), DecodeStr[what].c_str(),
//         glCommon::systemnanotime() - last);
}

DecodeThread::~DecodeThread() {
    quit();
}

DecodeThread::DecodeThread() : Looper("DecodeThread") {}

void DecodeThread::requestFrame(RenderResRequest *rrr) {
//    rrr->start = glCommon::systemnanotime();
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
        pRequest->task->setReadyPboRes(nullptr, pRequest->resIndex);
        mUploader->dataFillFail(pRequest);
        return;
    }
    auto ptus = pRequest->task->getPresentTimeUs();
    auto start = pRequest->start;

    auto ok = pFs->requestFrame(this, [this, pFs, pRequest]GET_FRAME_CALLBACK {
//        auto ss = glCommon::systemnanotime();
//        LOGD("GET_FRAME_CALLBACK size = %d, ptUS %lld", size, info->presentationTimeUs);
        auto pbo = mUploader->uploadFrame(buf, (size_t) size);
//        LOGV("upload take = %lld", glCommon::systemnanotime() - ss);
        if (pbo != nullptr) {
            pbo->width = pFs->getWidth();
            pbo->height = pFs->getHeight();
            pRequest->task->setReadyPboRes(pbo, pRequest->resIndex);
            mUploader->dataFillSuccess(pRequest);
//            LOGV("after dataFillSuccess take = %lld", glCommon::systemnanotime() - ss);
        } else {
            pRequest->task->setReadyPboRes(nullptr, pRequest->resIndex);
            mUploader->dataFillFail(pRequest);
        }
//        LOGV("out callback upload take = %lld", glCommon::systemnanotime() - ss);
    });
//    LOGD("after requestFrame take = %lld", glCommon::systemnanotime() - start);

    if (!ok) {
        requestFrame(pRequest);
    }
//
//    LOGV("out handleRequestFrame index %lld , take %lld , ok = %d",
//         ptus,
//         glCommon::systemnanotime() - start, ok);
//    LOGV("handleRequestFrame  %s   %d", pFs->getName().c_str(), index);
}

void DecodeThread::prepareRes(IFrameSource *pSource) {
    post(kWhatPrepareRes, pSource);
}

void DecodeThread::queueInputBuffer(VideoFrameSource *pVfs) {
    post(kWhatQueueInputBuffer, pVfs);
}

void DecodeThread::bindUploader(Uploader *uploader) {
    mUploader = uploader;
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
    } else {
        mUploader->dataFillFail(pReq->rrr);

        delete pReq;
    }
}

void DecodeThread::releaseRes(IFrameSource *pSource) {
    post(kWhatReleaseRes, pSource);
}



