//
// Created by ranqingguo on 7/7/17.
//

#include <sstream>
#include "EffectLine.h"

typedef struct EffectItem {
    Effect *pEffect;

    EffectItem *next;
    EffectItem *prev;
} EffectItem;

EffectLine::EffectLine() {
    mHeader = new EffectItem;

    mHeader->prev = nullptr;
    mHeader->pEffect = nullptr;

    mEnd = new EffectItem;
    mEnd->prev = mHeader;
    mEnd->next = nullptr;
    mEnd->pEffect = nullptr;

    mHeader->next = mEnd;
}

bool EffectLine::checkNoTimelineGap() {
    if (mHeader == nullptr)
        return false;

    auto current = mHeader;
    while (true) {
        auto next = mHeader->next;
        if (next == nullptr) {
            return true;
        }

        if (current->pEffect->getEndTimeUs() != next->pEffect->getStartTimeUs()) {
            return false;
        }
    }
}

void EffectLine::append(Effect *effect) {
    if (effect == nullptr)
        return;

    auto ei = new EffectItem();
    ei->pEffect = effect;
    if (mEnd->prev != mHeader) {
        effect->offsetTime(mEnd->prev->pEffect->getEndTimeUs());
    }
    appendEffectItem(ei, mEnd->prev);

}

void EffectLine::appendPre(Effect *effect) {
    if (effect == nullptr)
        return;

    auto ei = new EffectItem;

    ei->pEffect = effect;

    auto offset = effect->getDurationUs();
    auto ce = mHeader->next;
    while (ce != mEnd) {
        ce->pEffect->offsetTime(offset);
        ce = ce->next;
    }

    appendEffectItem(ei, mHeader);
}

void EffectLine::appendEffectItem(EffectItem *item, EffectItem *ei) {
    auto next = ei->next;

    ei->next = item;
    item->prev = ei;

    next->prev = item;
    item->next = next;
}


bool EffectLine::advance() {
    mCurrent = mCurrent->next;
    return mCurrent != mEnd;
}

Effect *EffectLine::getCurrent() {
    if (mCurrent == nullptr)
        mCurrent = mHeader->next;
    return mCurrent->pEffect;
}

void EffectLine::reset() {
    mCurrent = mHeader->next;
}

void EffectLine::seekTo(long timeUs) {

}

std::string EffectLine::getInfo() {
    std::stringstream ss;

    auto ei = mHeader->next;
    while (ei != nullptr && ei != mEnd) {
        auto ef = ei->pEffect;
        ss << ef->getStartTimeUs() << "  " << ef->getEndTimeUs() << std::endl;

        ei = ei->next;
    }

    return ss.str();
}
