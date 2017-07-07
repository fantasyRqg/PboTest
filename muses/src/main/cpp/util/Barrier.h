//
// Created by ranqingguo on 7/4/17.
//

#ifndef MUSES_BARRIER_H
#define MUSES_BARRIER_H

#include <cstdlib>
#include <condition_variable>
#include <mutex>

class Barrier {
public:
    explicit Barrier(std::size_t iCount) :
            mThreshold(iCount),
            mCount(iCount),
            mGeneration(0) {
    }

    void Wait() {
        std::unique_lock<std::mutex> lLock{mMutex};
        auto lGen = mGeneration;
        if (!--mCount) {
            mGeneration++;
            mCount = mThreshold;
            mCond.notify_all();
        } else {
            mCond.wait(lLock, [this, lGen] { return lGen != mGeneration; });
        }
    }

private:
    std::mutex mMutex;
    std::condition_variable mCond;
    std::size_t mThreshold;
    std::size_t mCount;
    std::size_t mGeneration;
};

#endif //MUSES_BARRIER_H
