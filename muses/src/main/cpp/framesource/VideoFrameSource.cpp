//
// Created by ranqingguo on 7/5/17.
//


#include <thread>
#include <media/NdkMediaCodec.h>
#include <android/native_window_jni.h>

#include "VideoFrameSource.h"
#include "DecodeThread.h"
#include "../util/common.h"


#undef TAG
#define  TAG "VideoFrameSource"

#define CODEC_WAIT_TIMEOUT 100


VideoFrameSource::VideoFrameSource(std::string videoPath)
        : IFrameSource(-1, -1), mVideoPath(videoPath) {
    mExtractor = AMediaExtractor_new();
    AMediaExtractor_setDataSource(mExtractor, mVideoPath.c_str());
    auto trackCount = AMediaExtractor_getTrackCount(mExtractor);

    std::string videoPrefix = "video/";

    const char *buf = new char[128];
    for (size_t i = 0; i < trackCount; ++i) {
        auto format = AMediaExtractor_getTrackFormat(mExtractor, i);
        AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &buf);

        std::string mimetype(buf);
        if (mimetype.find(videoPrefix) != std::string::npos) {
            AMediaExtractor_selectTrack(mExtractor, i);

            mMimeType = buf;
            initMediaInfo(format);

            return;
        }

        AMediaFormat_delete(format);
    }

    delete[] buf;

    throw std::runtime_error("init video frame source failure");

}

bool VideoFrameSource::prepare(DecodeThread *decodeThread) {
    startMediaCodec();
    while (queueInputBuffer() >= 0) {}
//    decodeThread->queueInputBuffer(this);
    return true;
}

bool VideoFrameSource::requestFrame(DecodeThread *decodeThread, GetFrameCallback &&callback) {
    auto index = dequeueOutputBuffer(callback);
    decodeThread->queueInputBuffer(this);
    return index >= 0;
}


bool VideoFrameSource::isOk() {
    auto ok = mEndUs != -1 && mFrameRate != -1 && mStartUs != -1;
    return ok;
}

VideoFrameSource::~VideoFrameSource() {
    if (mExtractor != nullptr)
        AMediaExtractor_delete(mExtractor);
    if (mFormat != nullptr)
        AMediaFormat_delete(mFormat);

    if (mDecoder != nullptr) {
        AMediaCodec_stop(mDecoder);
        AMediaCodec_delete(mDecoder);
    }

    if (mMimeType != nullptr) {
        delete[] mMimeType;
    }
}

void VideoFrameSource::initMediaInfo(AMediaFormat *format) {
    mFormat = format;
    mStartUs = 0;
    AMediaFormat_getInt64(format, AMEDIAFORMAT_KEY_DURATION, (int64_t *) &mEndUs);
    AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, &mFrameRate);
    AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_WIDTH, &mWidth);
    AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_HEIGHT, &mHeight);
    if (!isOk()) {
        throw std::runtime_error("init video frame source failure");
    }
}

int VideoFrameSource::getFrameRate() const {
    return mFrameRate;
}


void VideoFrameSource::startMediaCodec() {
    if (!mDecoderRun) {
        mDecoder = AMediaCodec_createDecoderByType(mMimeType);
        AMediaCodec_configure(mDecoder, mFormat, nullptr, nullptr, 0);
        AMediaCodec_start(mDecoder);
        mDecoderRun = true;
    }

    AMediaExtractor_seekTo(mExtractor, 0, AMEDIAEXTRACTOR_SEEK_NEXT_SYNC);
    mSeeOutputEos = false;
    mSeeOutputEos = false;
}

ssize_t VideoFrameSource::queueInputBuffer() {
    auto decoder = mDecoder;
    auto extractor = mExtractor;
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
            mSeeInputEos = true;
            AMediaCodec_queueInputBuffer(decoder, (size_t) index, 0, 0, 0,
                                         AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            return -1;
        }

        AMediaExtractor_advance(mExtractor);
    }

    return index;
}

int VideoFrameSource::dequeueOutputBuffer(GetFrameCallback callback) {
    if (!mDecoderRun) {
        return -1;
    }
    auto decoder = mDecoder;
    AMediaCodecBufferInfo info;
    auto index = AMediaCodec_dequeueOutputBuffer(decoder, &info, CODEC_WAIT_TIMEOUT);
    if (index >= 0) {
        size_t size = 0;
        auto buf = AMediaCodec_getOutputBuffer(decoder, (size_t) index, &size);

        if (callback != nullptr && size > 0)
            callback(index, buf, size, &info);
        AMediaCodec_releaseOutputBuffer(decoder, (size_t) index, false);

        if ((info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) != 0) {
            mSeeOutputEos = true;
        }
    }

    if (index == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
        auto format = AMediaCodec_getOutputFormat(decoder);

        LOGI("format:%s", AMediaFormat_toString(format));
    }
    return index;
}

bool VideoFrameSource::isPrepared() {
    return mDecoderRun && !mSeeOutputEos && !mSeeOutputEos;
}

bool VideoFrameSource::isVideo() {
    return true;
}

bool VideoFrameSource::skipOneFrame() {
    return dequeueOutputBuffer(nullptr) >= 0;
}

std::string VideoFrameSource::getName() {
    return mVideoPath;
}

bool VideoFrameSource::release() {
    auto stopR = AMediaCodec_stop(mDecoder);
    auto delR = AMediaCodec_delete(mDecoder);
    mDecoder = nullptr;
    mDecoderRun = false;
    return stopR == AMEDIA_OK && delR == AMEDIA_OK;
}
