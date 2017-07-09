//
// Created by ranqingguo on 7/7/17.
//

#include "Painter.h"
#include "Uploader.h"

enum {
    kWhatDrawRenderTask,
};

void Painter::postDrawRenderTask(RenderTask *task) {
    post(kWhatDrawRenderTask, task);
}

void Painter::handle(int what, void *data) {

    switch (what) {
        case kWhatDrawRenderTask:
            handleDrawRenderTask((RenderTask *) data);
            break;


        default:
            break;
    }
}

void Painter::handleDrawRenderTask(RenderTask *pTask) {
    pTask->draw();


    int size = 0;
    pTask->getFrameSourceArray(&size);

    for (int i = 0; i < size; ++i) {
        mUploader->releaseBuffer(pTask->getPboResAt(i));
    }
}

void Painter::bindUploader(Uploader *uploader) {
    mUploader = uploader;
}
