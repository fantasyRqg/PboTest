//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_VIDEOFRAMESOURCE_H
#define MUSES_VIDEOFRAMESOURCE_H

#include <string>
#include <media/NdkMediaExtractor.h>

#include "IFrameSource.h"
#include "../NativeSurfaceTexture.h"
//#include "DecodeThread.h"


class DecodeThread;

class VideoFrameSource : public IFrameSource {
public:

    /**
     * start time auto = 0
     *
     * end time = durationMills;
     */
    VideoFrameSource(std::string videoPath);

    ~VideoFrameSource();

    int getFrameRate() const;

    bool isOk() override;

    bool isPrepared() override;

    bool isVideo() override;

    bool skipOneFrame() override;

    bool prepare(DecodeThread *decodeThread) override;


    std::string getName() override;

    bool release() override;


    ssize_t queueInputBuffer();

    bool requestFrame(DecodeThread *decodeThread, RenderResRequest *pReq) override;

    bool bindTexture(JNIEnv *env, int textName, void *other) override;

    bool unbindTexture(JNIEnv *env) override;

    void updateImage(JNIEnv *env) override;

private:


    void initMediaInfo(AMediaFormat *format);


    int mFrameRate;
    std::string mVideoPath;
    AMediaExtractor *mExtractor;
    AMediaFormat *mFormat;
    AMediaCodec *mDecoder;
    const char *mMimeType = nullptr;
    bool mDecoderRun = false;
    NativeSurfaceTexture *mSurfaceTexture;

    bool mSeeOutputEos;
    bool mSeeInputEos;

    void startMediaCodec();

    int dequeueOutputBuffer(RenderResRequest *pReq);
};


#endif //MUSES_VIDEOFRAMESOURCE_H
