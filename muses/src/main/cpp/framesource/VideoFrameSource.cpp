//
// Created by ranqingguo on 7/5/17.
//

#include <thread>
#include "VideoFrameSource.h"

VideoFrameSource::VideoFrameSource(std::string &videoPath)
        : IFrameSource(-1, -1), mVideoPath(videoPath) {
    mExtractor = AMediaExtractor_new();
    AMediaExtractor_setDataSource(mExtractor, mVideoPath.data());
    auto trackCount = AMediaExtractor_getTrackCount(mExtractor);

    std::string videoPrefix = "video/";

    const char *buf = new char[128];
    for (size_t i = 0; i < trackCount; ++i) {
        auto format = AMediaExtractor_getTrackFormat(mExtractor, i);
        AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &buf);

        std::string mimetype(buf);
        if (mimetype.find(videoPrefix) != std::string::npos) {
            AMediaExtractor_selectTrack(mExtractor, i);

            initMediaInfo(format);

            delete buf;
            return;
        }

        AMediaFormat_delete(format);
    }

    delete buf;

    throw std::runtime_error("init video frame source failure");

}

bool VideoFrameSource::prepare() {
    return false;
}

bool VideoFrameSource::requestFrame() {
    return false;
}

bool VideoFrameSource::isOk() {
    auto ok = mEndUs != -1 && mFrameRate != -1 && mStartUs != -1;
    return ok;
}

VideoFrameSource::~VideoFrameSource() {
    AMediaExtractor_delete(mExtractor);
    AMediaFormat_delete(mFormat);
}

void VideoFrameSource::initMediaInfo(AMediaFormat *format) {
    mFormat = format;
    mStartUs = 0;
    AMediaFormat_getInt64(format, AMEDIAFORMAT_KEY_DURATION, &mEndUs);
    AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, &mFrameRate);

    if (!isOk()) {
        throw std::runtime_error("init video frame source failure");
    }
}

int VideoFrameSource::getFrameRate() const {
    return mFrameRate;
}

