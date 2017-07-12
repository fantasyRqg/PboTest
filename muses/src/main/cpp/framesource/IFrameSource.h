//
// Created by ranqingguo on 7/4/17.
//

#ifndef MUSES_IFRAMESOURCE_H
#define MUSES_IFRAMESOURCE_H

#include <algorithm>
#include <functional>
#include <media/NdkMediaCodec.h>
#include <jni.h>

//#define GET_FRAME_CALLBACK (int index, void *buf, int size, AMediaCodecBufferInfo *info) -> void

//typedef std::function<void(int index, void *buf, int size,
//                           AMediaCodecBufferInfo *info)> GetFrameCallback;

class DecodeThread;

struct RenderResRequest;

class IFrameSource {

public:
    /**
     * specify start and end time
     * @param startMills
     * @param endMills
     */
    IFrameSource(long startMills, long endMills);

    /**
     * run on painter
     * @param env
     * @param textName
     * @param other
     * @return
     */
    virtual bool bindTexture(JNIEnv *env, int textName, void *other) = 0;

    /**
     * run on painter
     * @param env
     * @return
     */
    virtual bool unbindTexture(JNIEnv *env) = 0;

    /**
     * run on painter
     * @param env
     */
    virtual void updateImage(JNIEnv *env) = 0;

    virtual bool prepare(DecodeThread *decodeThread) = 0;

    virtual bool release() = 0;

    virtual bool requestFrame(DecodeThread *decodeThread, RenderResRequest *pReq) = 0;

    virtual bool isOk() = 0;

    virtual bool isPrepared() = 0;

    virtual bool isVideo() = 0;

    virtual bool skipOneFrame() = 0;

    virtual std::string getName()= 0;


    long getStartUs() const;

    long getEndUs() const;

    int getWidth() const;

    int getHeight() const;


protected:
    long mStartUs = -1;
    long mEndUs = -1;

    int mWidth;
    int mHeight;

//    long mFrameStartMills;
//
//    long mFirstFrameUs;
//    /**
//     * some time effect will use more frame,so adjust duration
//     */
//    long mPlayStartMills;
//
//    /**
//     * some time effect will use more frame,so adjust duration
//     */
//    long mPlayEndMills;

};

#endif //MUSES_IFRAMESOURCE_H
