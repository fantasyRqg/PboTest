//
// Created by ranqingguo on 7/8/17.
//

#include "RenderTask.h"
#include "exception/IlleaglStateException.h"
#include "Uploader.h"

RenderTask::RenderTask(long presentTimeUs, IFrameSource **frameSourceArray, int FSArrayLen,
                       Render **renderArray, int renderArrayLen) :
        mPresentTimeUs(presentTimeUs),
        mFrameSourceArray(frameSourceArray),
        mFSArrayLen(FSArrayLen),
        mRenderArray(renderArray),
        mRenderArrayLen(renderArrayLen) {

    auto c = 0;

    for (int i = 0; i < mRenderArrayLen; ++i) {
        c += mRenderArray[i]->getNeededPboCount();
    }

    if (c != mFSArrayLen) {
        throw IllegalStateException("needed bpo count not  === provided frame source count");
    }


    mPboResArray = new PboRes *[mFSArrayLen];
    mSkipFrameArray = new int[mFSArrayLen];

    for (int i = 0; i < mFSArrayLen; ++i) {
        mPboResArray[i] = nullptr;
        mSkipFrameArray[i] = 0;
    }
}

void RenderTask::draw() {
    for (int i = 0; i < mRenderArrayLen; ++i) {
        mRenderArray[i]->drawFrame(mPresentTimeUs);
    }
}

void RenderTask::prepareDrawFrame() {
    for (int i = 0; i < mRenderArrayLen; ++i) {
        mRenderArray[i]->prepareDrawFrame();
    }
}

void RenderTask::setDrawPboRes() {
    auto fsCount = 0;
    for (int i = 0; i < mRenderArrayLen; ++i) {
        mRenderArray[i]->setDrawPboRes(mPboResArray + fsCount,
                                       mRenderArray[i]->getNeededPboCount());
    }

    mPboResArray[0]->sync;
}

RenderTask::~RenderTask() {
    if (mPboResArray != nullptr)
        delete[] mPboResArray;

    if (mSkipFrameArray != nullptr)
        delete[] mSkipFrameArray;
}

bool RenderTask::isTaskValid() {
    for (int i = 0; i < mFSArrayLen; ++i) {
        if (!mPboResArray[i]->isReady()) {
            return false;
        }
    }
    return true;
}

IFrameSource **RenderTask::getFrameSourceArray(int *out_size) {
    *out_size = mFSArrayLen;
    return mFrameSourceArray;
}

IFrameSource *RenderTask::getFrameSourceAt(int index) {
    if (index >= mFSArrayLen) {
        return nullptr;
    }

    return mFrameSourceArray[index];
}

PboRes *RenderTask::getPboResAt(int index) {
    if (index >= mFSArrayLen) {
        return nullptr;
    }
    return mPboResArray[index];
}

int RenderTask::getSkipFrameAt(int index) {
    if (index >= mFSArrayLen) {
        return 0;
    }

    return mSkipFrameArray[index];
}

void RenderTask::setReadyPboRes(PboRes *pRes, int i) {
    mPboResArray[i] = pRes;
}
