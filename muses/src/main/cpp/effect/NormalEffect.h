//
// Created by ranqingguo on 7/13/17.
//

#ifndef PBOTEST_NORMALEFFECT_H
#define PBOTEST_NORMALEFFECT_H


#include "Effect.h"

class NormalEffect : public Effect {
public:
    NormalEffect(std::string videoPath, long durationUs);

    RenderTask *nextRenderTask() override;


};


#endif //PBOTEST_NORMALEFFECT_H
