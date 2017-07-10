//
// Created by ranqingguo on 7/10/17.
//

#ifndef PBOTEST_TESTEFFECT_H
#define PBOTEST_TESTEFFECT_H


#include "Effect.h"

class TestEffect : public Effect {
public:
    TestEffect(long durationUs);

    RenderTask *nextRenderTask() override;

};


#endif //PBOTEST_TESTEFFECT_H
