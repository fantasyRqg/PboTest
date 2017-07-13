//
// Created by ranqingguo on 7/13/17.
//

#ifndef PBOTEST_NORMALEFFECT_H
#define PBOTEST_NORMALEFFECT_H


#include "Render.h"

class NormalRender : Render {

    GLuint getTextureIdAt(int index) override;

    int getTextureCount() override;

    void drawFrame(int64_t timeUs) override;

    bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) override;

    bool tearDown() override;

};


#endif //PBOTEST_NORMALEFFECT_H
