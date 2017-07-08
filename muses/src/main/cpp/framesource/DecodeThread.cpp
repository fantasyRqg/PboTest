//
// Created by ranqingguo on 7/7/17.
//

#include "DecodeThread.h"
#include "../RenderTask.h"
#include "../Uploader.h"

enum {
    kWhatQueueInputBuffer,
    kWhatDequeueOutputBuffer,
    kWhatSeek,
    kWhatReleaseRes,
    kWhatPrepareRes,
    kWhatRequestFrame,
    kWhatSkipFrame,
};

void DecodeThread::handle(int what, void *data) {
    auto pVfs = (VideoFrameSource *) data;
    switch (what) {
        case kWhatPrepareRes:
            break;
        case kWhatDequeueOutputBuffer:

            break;
        case kWhatQueueInputBuffer: {
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

        case kWhatReleaseRes:
            AMediaCodec_stop(pVfs->mDecoder);
            AMediaCodec_delete(pVfs->mDecoder);
            pVfs->mDecoder = nullptr;
            pVfs->mDecoderRun = false;
            break;

        case kWhatSkipFrame:

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
    pFs->requestFrame(this, [this, pRequest]GET_FRAME_CALLBACK {
        auto pbo = mUploader->uploadFrame(buf, (size_t) size);
        pRequest->task->setReadyPboRes(pbo, pRequest->resIndex);
    });
}

void DecodeThread::prepareRes(IFrameSource *pSource) {
    post(kWhatPrepareRes, pSource);
}

void DecodeThread::queueInputBuffer(VideoFrameSource *pVfs) {
    post(kWhatQueueInputBuffer, pVfs);
}



