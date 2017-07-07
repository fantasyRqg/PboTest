//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_DECODETHREAD_H
#define PBOTEST_DECODETHREAD_H


#include <thread>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaCodec.h>

#include "util/Looper.h"
#include "../util/Looper.h"
#include "VideoFrameSource.h"

class DecodeThread : public Looper {

public:
    DecodeThread(AMediaFormat *format, AMediaExtractor *extractor);

    virtual ~DecodeThread();

    void handle(int what, void *data) override;

private:
    VideoFrameSource *mFrameSource = nullptr;
    AMediaFormat *mFormat = nullptr;
    AMediaExtractor *mExtractor = nullptr;
    AMediaCodec *mDecoder = nullptr;

};


#endif //PBOTEST_DECODETHREAD_H
