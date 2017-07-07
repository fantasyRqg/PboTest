//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_VIDEOFRAMESOURCE_H
#define MUSES_VIDEOFRAMESOURCE_H

#include <string>
#include <media/NdkMediaExtractor.h>

#include "IFrameSource.h"
#include "DecodeThread.h"

class DecodeThread;

class VideoFrameSource : public IFrameSource {
public:

    /**
     * start time auto = 0
     *
     * end time = durationMills;
     */
    VideoFrameSource(std::string &videoPath);

    ~VideoFrameSource();

    int getFrameRate() const;

    bool prepare() override;

    bool requestFrame() override;

    bool isOk() override;

    friend class DecodeThread;

private:
    void initMediaInfo(AMediaFormat *format);

    void onDequeueOutpuBuffer();

    int mFrameRate;
    std::string mVideoPath;
    AMediaExtractor *mExtractor;
    AMediaFormat *mFormat;
    AMediaCodec *mDecoder;
    const char *mMimeType = nullptr;
    bool mDecoderRun = false;

};


#endif //MUSES_VIDEOFRAMESOURCE_H
