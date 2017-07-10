//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_EFFECTMANAGER_H
#define PBOTEST_EFFECTMANAGER_H

#include <vector>


#include "EffectLine.h"

/**
 * responsible for manger for depend on  effect line and timeline change RenderManager struct
 * ,for current render
 */
class EffectManager {

public:
    EffectManager(int outFrameRate, EffectLine *pBaseEffectLine);

    virtual ~EffectManager();

    int getOutFrameRate() const;

    /**
     * to next effect
     * @return has next effect
     */
    bool advance();

    Effect *getEffect();

    void reset();

private:

    int mOutFrameRate;

    /**
     * time line is no gap
     */
    EffectLine *mBaseEffectLine;

    std::vector<long> mModieyTimes;
};


#endif //PBOTEST_EFFECTMANAGER_H
