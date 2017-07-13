//
// Created by ranqingguo on 7/13/17.
//

#ifndef PBOTEST_NORMALRENDER_H
#define PBOTEST_NORMALRENDER_H


#include <glm/glm.hpp>
#include "Render.h"

class NormalRender : public Render {
public:

    int getTextureCount() override;

    void drawFrame(int64_t timeUs) override;

    bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) override;

    bool tearDown() override;

protected:
    GLuint *getTextureIdArray() override;

private:
    const static int TEXTURE_COUNT = 1;
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVbo;
    GLuint mIbo;

    GLuint mTextureIds[TEXTURE_COUNT];

    GLint mMvpLocation;
    GLint mVideoTexLoc;


    glm::mat4 mProjMatrix;
    glm::mat4 mViewMatrix;
};


#endif //PBOTEST_NORMALRENDER_H
