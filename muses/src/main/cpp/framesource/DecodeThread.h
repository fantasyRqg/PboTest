//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_DECODETHREAD_H
#define PBOTEST_DECODETHREAD_H


#include <thread>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaCodec.h>

#include "../util/Looper.h"
#include "VideoFrameSource.h"

class DecodeThread : public Looper {

public:
    DecodeThread();

    virtual ~DecodeThread();

    void handle(int what, void *data) override;

private:


    void startMediaCodec(VideoFrameSource *pVfs);

    ssize_t queueInputBuffer(VideoFrameSource *pVfs);
};


#endif //PBOTEST_DECODETHREAD_H
