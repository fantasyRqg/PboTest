//
// Created by ranqingguo on 7/7/17.
//

#include "DecodeThread.h"

enum {
    kWhatQueueInputBuffer,
    kWhatDequeueOutputBuffer,
    kWhatSeek,
    kWhatDecodeDone,
    kWhatStartNewDecode
};


void DecodeThread::handle(int what, void *data) {
    switch (what) {
        case kWhatStartNewDecode:

            break;
        case kWhatDequeueOutputBuffer:

            break;
        case kWhatQueueInputBuffer:
            break;

        case kWhatSeek:
            break;

        case kWhatDecodeDone:

            break;
    }
}

DecodeThread::~DecodeThread() {


}

DecodeThread::DecodeThread() {}

