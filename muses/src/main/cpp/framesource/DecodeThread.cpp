//
// Created by ranqingguo on 7/7/17.
//

#include <sstream>
#include "DecodeThread.h"
#include "../RenderTask.h"
#include "../Uploader.h"

enum {
    kWhatQueueInputBuffer,
    kWhatSeek,
    kWhatReleaseRes,
    kWhatPrepareRes,
    kWhatRequestFrame,
    kWhatSkipFrame,
};


typedef struct SkipReq {
    RenderResRequest *rrr;
    int skip;
} SkipReq;

void DecodeThread::handle(int what, void *data) {
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
            if (index >= 0) {
                post(kWhatQueueInputBuffer, pVfs);
            }
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

}

DecodeThread::~DecodeThread() {
    quit();
}

DecodeThread::DecodeThread() {}

void DecodeThread::requestFrame(RenderResRequest *rrr) {
    post(kWhatRequestFrame, rrr);

}

void DecodeThread::handleRequestFrame(RenderResRequest *pRequest) {
    auto pFs = pRequest->task->getFrameSourceAt(pRequest->resIndex);
    auto skip = pRequest->task->getSkipFrameAt(pRequest->resIndex);

    if (skip > 0) {
        auto sr = new SkipReq();
        sr->rrr = pRequest;
        sr->skip = skip;
        post(kWhatSkipFrame, &sr);
        return;
    }

    pFs->requestFrame(this, [this, pRequest]GET_FRAME_CALLBACK {
        auto pbo = mUploader->uploadFrame(buf, (size_t) size);
        if (pbo != nullptr) {
            pRequest->task->setReadyPboRes(pbo, pRequest->resIndex);
            mUploader->dataFillSuccess(pRequest);
        } else {
            pRequest->task->setReadyPboRes(nullptr, pRequest->resIndex);
            mUploader->dataFillFail(pRequest);
        }
    });
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



