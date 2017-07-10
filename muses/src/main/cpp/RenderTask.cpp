//
// Created by ranqingguo on 7/8/17.
//

#include "RenderTask.h"
#include "exception/IlleaglStateException.h"
#include "Uploader.h"


RenderTask::RenderTask(long presentTimeUs,
                       const std::vector<std::shared_ptr<IFrameSource>> &frameSourceVector,
                       const std::vector<std::shared_ptr<Render>> &renderVector) : mPresentTimeUs(
        presentTimeUs), mFrameSourceVector(frameSourceVector), mRenderVector(renderVector) {
    auto c = 0;

    for (int i = 0; i < mRenderVector.size(); ++i) {
        c += mRenderVector[i]->getNeededPboCount();
    }

    unsigned int size = mFrameSourceVector.size();
    if (c != size) {
        throw IllegalStateException("needed bpo count not  === provided frame source count");
    }


    mPboResArray = new PboRes *[size];
    mSkipFrameArray = new int[size];
    mProcessedMark = new bool[size];

    for (int i = 0; i < size; ++i) {
        mPboResArray[i] = nullptr;
        mSkipFrameArray[i] = 0;
        mProcessedMark[i] = false;
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
    if (mPboResArray != nullptr)
        delete[] mPboResArray;

    if (mSkipFrameArray != nullptr)
        delete[] mSkipFrameArray;

    if (mProcessedMark != nullptr) {
        delete[] mProcessedMark;
    }
}

bool RenderTask::isTaskValid() {
    for (int i = 0; i < mFrameSourceVector.size(); ++i) {
        if (mPboResArray[i] == nullptr || !mPboResArray[i]->isReady()) {
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

PboRes *RenderTask::getPboResAt(int index) {
    if (index >= mFrameSourceVector.size()) {
        return nullptr;
    }
    return mPboResArray[index];
}

int RenderTask::getSkipFrameAt(int index) {
    if (index >= mFrameSourceVector.size()) {
        return 0;
    }

    return mSkipFrameArray[index];
}

void RenderTask::setReadyPboRes(PboRes *pRes, int i) {
    mPboResArray[i] = pRes;
    mProcessedMark[i] = true;
}

bool RenderTask::isAllResProcessed() {
    for (int i = 0; i < mFrameSourceVector.size(); ++i) {
        if (!mProcessedMark[i]) {
            return false;
        }
    }
    return true;
}


//
//bool RenderTask::isSkipped() const {
//    return mSkipped;
//}
//
//void RenderTask::setSkipped(bool skipped) {
//    mSkipped = skipped;
//}
