//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_PBOTESTRENDER_H
#define MUSES_PBOTESTRENDER_H


#include "Render.h"

class PboTestRender : public Render {
public:
    void prepareNextFrame() override;

    void drawFrame(long millsecond) override;

    bool setUp(AAssetManager *amgr) override;

    bool tearDown() override;


private:
    GLuint mProgramId;
    GLuint mVertexArrayId;
    GLuint mVertexBufferId;

    GLuint mPbo;

    void createBpo(size_t imageSize, const GLuint *pbo, int index) const;
};


#endif //MUSES_PBOTESTRENDER_H
