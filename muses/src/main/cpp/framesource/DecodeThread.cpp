//
// Created by ranqingguo on 7/7/17.
//

#include "DecodeThread.h"

#define CODEC_WAIT_TIMEOUT 200000

enum {
    kWhatQueueInputBuffer,
    kWhatDequeueOutputBuffer,
    kWhatSeek,
    kWhatDecodeDone,
    kWhatStartNewDecode
};


void DecodeThread::handle(int what, void *data) {
    auto pVfs = (VideoFrameSource *) data;
    switch (what) {
        case kWhatStartNewDecode:
            startMediaCodec(pVfs);
            break;
        case kWhatDequeueOutputBuffer:

            break;
        case kWhatQueueInputBuffer:
            auto index = queueInputBuffer(pVfs);
            if (index >= 0) {
                post(kWhatQueueInputBuffer, pVfs);
            }
            break;

        case kWhatSeek:
            break;

        case kWhatDecodeDone:

            break;
    }
}

DecodeThread::~DecodeThread() {


}

DecodeThread::DecodeThread() {}

void DecodeThread::startMediaCodec(VideoFrameSource *pVfs) {
    pVfs->mDecoder = AMediaCodec_createDecoderByType(pVfs->mMimeType);
    AMediaCodec_configure(pVfs->mDecoder, pVfs->mFormat, nullptr, nullptr, 0);
    AMediaCodec_start(pVfs->mDecoder);
}

ssize_t DecodeThread::queueInputBuffer(VideoFrameSource *pVfs) {
    auto decoder = pVfs->mDecoder;
    auto extractor = pVfs->mExtractor;
    auto index = AMediaCodec_dequeueInputBuffer(decoder, CODEC_WAIT_TIMEOUT);

    if (index >= 0) {
        size_t buffSize;

        auto buf = AMediaCodec_getInputBuffer(decoder, (size_t) index, &buffSize);
        auto sampleSize = AMediaExtractor_readSampleData(extractor, buf, buffSize);
        auto ptUs = AMediaExtractor_getSampleTime(extractor);
        auto flag = AMediaExtractor_getSampleFlags(extractor);

        if (sampleSize > 0) {
            AMediaCodec_queueInputBuffer(decoder, (size_t) index, 0, (size_t) sampleSize,
                                         (uint64_t) ptUs, flag);
        } else {
            AMediaCodec_queueInputBuffer(decoder, (size_t) index, 0, 0, 0,
                                         AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            post(kWhatDecodeDone, pVfs);
            return -1;
        }
    }

    return index;
}

