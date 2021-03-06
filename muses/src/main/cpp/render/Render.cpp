//
// Created by ranqingguo on 7/4/17.
//

#include <cstdlib>
#include <string.h>
#include "Render.h"
#include "../util/common.h"

#undef TAG
#define TAG "EffectRender"


GLuint Render::loadShader(AAssetManager *amgr, const char *fname, GLenum type) {

    GLuint id = 0;
    id = glCreateShader(type);

    //open file
    AAsset *asset = AAssetManager_open(amgr, fname, AASSET_MODE_UNKNOWN);
    if (!asset) {
        LOGE("asset get  failure");
        return 0;
    }
    size_t size = (size_t) AAsset_getLength(asset);
    GLchar *buffer = (GLchar *) malloc(size + 1);
    memset(buffer, 0, size + 1);

    if (!AAsset_read(asset, buffer, size)) {
        LOGE("read shader file error");
        return 0;
    }

    //set shader source
    glShaderSource(id, 1, (const GLchar *const *) &buffer, NULL);

    //close file
    AAsset_close(asset);
    free(buffer);

    //compile shader
    glCompileShader(id);

    //check error
    if (glCommon::checkShaderAndPrint(id)) {
        return id;
    } else {
        LOGE("shader name %s", fname);
        return 0;
    }
}

void Render::setZOrder(int ZOrder) {
    mZOrder = ZOrder;
}

int Render::getZOrder() const {
    return mZOrder;
}

bool Render::isRenderFbo() const {
    return mRenderFbo;
}

void Render::setRenderFbo(bool renderFbo) {
    mRenderFbo = renderFbo;
}

GLuint Render::getTextureIdAt(int index) {
    if (index > getTextureCount()) {
        return GL_NONE;
    }
    return getTextureIdArray()[index];
}

