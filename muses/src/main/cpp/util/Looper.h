//
// Created by ranqingguo on 7/6/17.
//

#ifndef MUSES_LOOPER_H
#define MUSES_LOOPER_H

#include <algorithm>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

struct LooperMessage;

class Looper {
public:
    Looper();

    Looper &operator=(const Looper &) = delete;

    Looper(Looper &) = delete;

    virtual ~Looper();

    void post(int what, void *data);


    void quit();

    virtual void handle(int what, void *data) = 0;



private:
    void tailAppendMessage(LooperMessage *message);

    void loop();

    std::queue<LooperMessage *> mMsgQueue;

    std::thread *mWorkThread;

    std::mutex mQueueLock;
    std::condition_variable mNewMsgCv;


    bool mRunning;

};


#endif //MUSES_LOOPER_H
