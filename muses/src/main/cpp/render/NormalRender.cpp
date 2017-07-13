//
// Created by ranqingguo on 7/13/17.
//

#include "NormalRender.h"


int NormalRender::getTextureCount() {

    return TEXTURE_COUNT;
}

void NormalRender::drawFrame(int64_t timeUs) {

}

bool NormalRender::setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) {
    return false;
}

bool NormalRender::tearDown() {
    return false;
}
