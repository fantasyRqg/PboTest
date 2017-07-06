//
// Created by ranqingguo on 7/5/17.
//

#ifndef PBOTEST_PBOTESTRENDER_H
#define PBOTEST_PBOTESTRENDER_H


#include "EffectRender.h"

class PboTestRender : public EffectRender {
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


#endif //PBOTEST_PBOTESTRENDER_H
