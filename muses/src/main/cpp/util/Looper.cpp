//
// Created by ranqingguo on 7/6/17.
//

#include "Looper.h"
#include "common.h"

#undef TAG
#define TAG "Looper"

typedef struct LooperMessage {
    int what;
    void *data;
    bool quit;
} LooperMessage;


void Looper::loop() {
    while (mRunning) {
        std::unique_lock<std::mutex> lock(mQueueLock);
        while (mMsgQueue.size() == 0) {
            mNewMsgCv.wait(lock, [this]() { return mMsgQueue.size() != 0; });
        }

        auto msg = mMsgQueue.front();
        mMsgQueue.pop();
        lock.unlock();

        LOGD("%s what = %d ", mName.c_str(), msg->what);

        handle(msg->what, msg->data);

        LOGV("%s what = %d ", mName.c_str(), msg->what);

        if (msg->quit) {
            delete (msg);
            return;
        }

        delete msg;
    }
}

Looper::~Looper() {

}

void Looper::post(int what, void *data) {
    auto msg = new LooperMessage();
    msg->data = data;
    msg->what = what;
    msg->quit = false;

    tailAppendMessage(msg);
}

void Looper::quit() {
    if (mRunning) {
        postQuit();
        mWorkThread->join();
        delete (mWorkThread);
        mRunning = false;
    }

}

void Looper::postQuit() {
    auto msg = new LooperMessage();
    msg->data = nullptr;
    msg->what = -1;
    msg->quit = true;
    tailAppendMessage(msg);
}


void Looper::tailAppendMessage(LooperMessage *message) {
    LOGD("tailAppendMessage %s what = %d", mName.c_str(), message->what);

    std::unique_lock<std::mutex> lock(mQueueLock);
    mMsgQueue.push(message);
    mNewMsgCv.notify_one();

    LOGV("tailAppendMessage %s what = %d ", mName.c_str(), message->what);

}

Looper::Looper(const std::string &name) : mName(name) {
    mRunning = true;
    mWorkThread = new std::thread([this]() -> void {
        loop();
    });
}

