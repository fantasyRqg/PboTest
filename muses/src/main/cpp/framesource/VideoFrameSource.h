//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_VIDEOFRAMESOURCE_H
#define MUSES_VIDEOFRAMESOURCE_H

#include <string>
#include <media/NdkMediaExtractor.h>

#include "IFrameSource.h"
//#include "DecodeThread.h"


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

    bool isOk() override;

    bool isPrepared() override;

    bool isVideo() override;

    bool skipOneFrame() override;

    bool prepare(DecodeThread *decodeThread) override;

    bool requestFrame(DecodeThread *decodeThread, GetFrameCallback &&callback) override;

    std::string getName() override;


private:

    friend class DecodeThread;

    void initMediaInfo(AMediaFormat *format);

//    void onDequeueOutputBuffer();

    int mFrameRate;
    std::string mVideoPath;
    AMediaExtractor *mExtractor;
    AMediaFormat *mFormat;
    AMediaCodec *mDecoder;
    const char *mMimeType = nullptr;
    bool mDecoderRun = false;

    bool mSeeOutputEos;
    bool mSeeInputEos;

    void startMediaCodec();

    ssize_t queueInputBuffer();

    int dequeueOutputBuffer(GetFrameCallback callback);
};


#endif //MUSES_VIDEOFRAMESOURCE_H
