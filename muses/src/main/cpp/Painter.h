//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_PAINTER_H
#define PBOTEST_PAINTER_H

#include "util/Looper.h"
#include "RenderTask.h"


class Uploader;

/**
 * response for draw frame
 */
class Painter : public Looper {
public:
    void postDrawRenderTask(RenderTask *task);

    void handle(int what, void *data) override;

    void handleDrawRenderTask(RenderTask *pTask);

    void bindUploader(Uploader *uploader);

private:
    Uploader *mUploader;


};


#endif //PBOTEST_PAINTER_H
