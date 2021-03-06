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
#include <jni.h>

struct LooperMessage;

class Looper {
public:
    Looper(const std::string &name);

    Looper &operator=(const Looper &) = delete;

    Looper(Looper &) = delete;

    virtual ~Looper();

    void post(int what, void *data);


    virtual void quit();


    void postQuit();

protected:
    virtual void handle(int what, void *data) = 0;

    virtual void loop();

    std::string mName;
private:

    void tailAppendMessage(LooperMessage *message);

    std::queue<LooperMessage *> mMsgQueue;

    std::thread *mWorkThread;

    std::mutex mQueueLock;
    std::condition_variable mNewMsgCv;

    bool mRunning;



};


#endif //MUSES_LOOPER_H
