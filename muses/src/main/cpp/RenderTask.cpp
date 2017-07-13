//
// Created by ranqingguo on 7/8/17.
//

#include <assert.h>
#include "RenderTask.h"

enum {
    kFrameReady,
    kFrameFail,
    kFrameUnset,
};

RenderTask::RenderTask(int64_t presentTimeUs,
                       const std::vector<std::shared_ptr<IFrameSource>> &frameSourceVector,
                       const std::vector<std::shared_ptr<Render>> &renderVector) : mPresentTimeUs(
        presentTimeUs), mFrameSourceVector(frameSourceVector), mRenderVector(renderVector) {
    auto c = 0;

    for (int i = 0; i < mRenderVector.size(); ++i) {
        c += mRenderVector[i]->getTextureCount();
    }

    unsigned int size = mFrameSourceVector.size();
    if (c != size) {
//        throw IllegalStateException("needed bpo count not  === provided frame source count");
        assert(false);
    }


    mSkipFrameArray = new int[size];
    mFrameMark = new int[size];

    for (int i = 0; i < size; ++i) {
        mSkipFrameArray[i] = 0;
        mFrameMark[i] = kFrameUnset;
    }

}

void RenderTask::draw() {
    for (auto r : mRenderVector) {
        r->drawFrame(mPresentTimeUs);
    }

}
//
//void RenderTask::prepareDrawFrame() {
//    for (int i = 0; i < mRenderArrayLen; ++i) {
//        mRenderArray[i]->prepareDrawFrame();
//    }
//}

//void RenderTask::setDrawPboRes() {
//    auto fsCount = 0;
//    for (int i = 0; i < mRenderArrayLen; ++i) {
//        mRenderArray[i]->setDrawPboRes(mPboResArray + fsCount,
//                                       mRenderArray[i]->getNeededPboCount());
//    }
//
//    mPboResArray[0]->sync;
//}

RenderTask::~RenderTask() {

    if (mSkipFrameArray != nullptr)
        delete[] mSkipFrameArray;

    if (mFrameMark != nullptr) {
        delete[] mFrameMark;
    }
}

bool RenderTask::isFramePrepared() {
    for (int i = 0; i < mFrameSourceVector.size(); ++i) {
        if (mFrameMark[i] != kFrameReady) {
            return false;
        }
    }
    return true;
}

std::vector<std::shared_ptr<IFrameSource>> RenderTask::getFrameSourceVector() {
    return mFrameSourceVector;
}

IFrameSource *RenderTask::getFrameSourceAt(int index) {
    if (index >= mFrameSourceVector.size()) {
        return nullptr;
    }

    return mFrameSourceVector[index].get();
}


int RenderTask::getSkipFrameAt(int index) {
    if (index >= mFrameSourceVector.size()) {
        return 0;
    }

    return mSkipFrameArray[index];
}


bool RenderTask::isAllFrameProcessed() {
    for (int i = 0; i < mFrameSourceVector.size(); ++i) {
        if (mFrameMark[i] == kFrameUnset) {
            return false;
        }
    }
    return true;
}

int64_t RenderTask::getPresentTimeUs() const {
    return mPresentTimeUs;
}

void RenderTask::updateImage(JNIEnv *pEnv) {
    for (auto fs : mFrameSourceVector) {
        fs->updateImage(pEnv);
    }
}

void RenderTask::setFrameReady(bool ready, int i) {
    if (ready) {
        mFrameMark[i] = kFrameReady;
    } else {
        mFrameMark[i] = kFrameFail;
    };
}




//
//bool RenderTask::isSkipped() const {
//    return mSkipped;
//}
//
//void RenderTask::setSkipped(bool skipped) {
//    mSkipped = skipped;
//}
