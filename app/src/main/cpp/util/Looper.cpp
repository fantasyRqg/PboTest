//
// Created by ranqingguo on 7/6/17.
//

#include "Looper.h"

typedef struct LooperMessage {
    int what;
    void *data;
    bool quit;
} LooperMessage;

Looper::Looper() {
    mRunning = true;
    mWorkThread = new std::thread([this]() -> void {
        loop();
    });
}

void Looper::loop() {
    while (mRunning) {
        std::unique_lock<std::mutex> lock(mQueueLock);
        while (mMsgQueue.size() == 0) {
            mNewMsgCv.wait(lock, [this]() { return mMsgQueue.size() != 0; });
        }

        auto msg = mMsgQueue.front();
        mMsgQueue.pop();
        lock.unlock();

        handle(msg->what, msg->data);

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
        auto msg = new LooperMessage();
        msg->data = nullptr;
        msg->what = NULL;
        msg->quit = true;
        tailAppendMessage(msg);
        mWorkThread->join();
        delete (mWorkThread);
        mRunning = false;
    }

}


void Looper::tailAppendMessage(LooperMessage *message) {
    std::unique_lock<std::mutex> lock(mQueueLock);
    mMsgQueue.push(message);
    mNewMsgCv.notify_one();
}

