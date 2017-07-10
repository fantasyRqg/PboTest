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
    void prepareDrawFrame() override;

    void setDrawPboRes(PboRes **pboReses, int len) override;

    int getNeededPboCount() override;

    void drawFrame(long timeUs) override;

    bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) override;

    bool tearDown() override;

private:
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVbo;
    GLuint mIndicatesVbo[2];

    GLint mMvpLocation;

    glm::mat4 mProjMatrix;
    glm::mat4 mViewMatrix;
};


#endif //ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H
