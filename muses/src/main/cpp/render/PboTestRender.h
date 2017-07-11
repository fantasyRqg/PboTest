//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_PBOTESTRENDER_H
#define MUSES_PBOTESTRENDER_H


#include "Render.h"
#include "../gl/surface/EglSurfaceBase.h"

class PboTestRender : public Render {
public:

    void drawFrame(int64_t timeUs) override;

    bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) override;

    bool tearDown() override;

    void prepareDrawFrame() override;


    int getNeededPboCount() override;


private:
    GLuint mVertexArrayId;
//    GLuint mProgramId;
//    GLuint mVertexBufferId;
//
//    GLuint mPbo;

    void createBpo(size_t imageSize, const GLuint *pbo, int index) const;
};


#endif //MUSES_PBOTESTRENDER_H
