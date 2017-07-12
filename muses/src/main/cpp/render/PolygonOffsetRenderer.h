//
// Created by ranqingguo on 3/18/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H
#define ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H

#include <glm/glm.hpp>

#include "Render.h"
#include "../gl/surface/EglSurfaceBase.h"

#undef TAG
#define TAG "PolygonOffsetRenderer"

class PolygonOffsetRenderer : public Render {
public:
    PolygonOffsetRenderer();

    void prepareDrawFrame() override;


    int getTextureCount() override;

    void drawFrame(int64_t timeUs) override;

    bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) override;

    bool tearDown() override;

private:
    const static int TEXTURE_COUNT = 2;
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVbo;
    GLuint mIndicatesVbo[2];

    GLint mMvpLocation;

    glm::mat4 mProjMatrix;
    glm::mat4 mViewMatrix;
};


#endif //ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H
