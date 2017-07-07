//
// Created by ranqingguo on 7/7/17.
//

#include "Effect.h"


long Effect::getStartTimeMills() const {
    return mStartTimeMills;
}

long Effect::getEndTimeMills() const {
    return mEndTimeMills;
}

Effect::Effect(long startTimeMills, long endTimeMills) : mStartTimeMills(
        startTimeMills), mEndTimeMills(endTimeMills) {


}

Effect::Effect(long durationMills) {

}

bool Effect::offsetTime(long offsetMills) {
    if (mStartTimeMills + offsetMills < 0)
        return false;

    mStartTimeMills += offsetMills;
    mEndTimeMills += offsetMills;
    return true;
}