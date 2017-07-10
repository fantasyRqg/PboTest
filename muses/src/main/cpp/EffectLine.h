//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_EFFECTLINE_H
#define PBOTEST_EFFECTLINE_H

#include <vector>

#include "effect/Effect.h"

struct EffectItem;

class EffectLine {

public:
    EffectLine();

    bool checkNoTimelineGap();

    void append(Effect *effect);

    void appendPre(Effect *effect);

    bool advance();

    Effect *getCurrent();

    void reset();

    void seekTo(long timeUs);

private:
    void appendEffectItem(EffectItem *item, EffectItem *ei);

    EffectItem *mHeader = nullptr;

    EffectItem *mEnd = nullptr;

    EffectItem *mCurrent = nullptr;
};


#endif //PBOTEST_EFFECTLINE_H
