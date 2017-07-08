//
// Created by ranqingguo on 7/7/17.
//

#include "EffectLine.h"

typedef struct EffectItem {
    Effect *pEffect;

    EffectItem *next;
    EffectItem *prev;
} EffectItem;

EffectLine::EffectLine() {
    pHeader = new EffectItem;

    pHeader->prev = nullptr;
    pHeader->pEffect = nullptr;

    pEnd = new EffectItem;
    pEnd->prev = pHeader;
    pEnd->next = nullptr;
    pEnd->pEffect = nullptr;

    pHeader->next = pEnd;
}

bool EffectLine::checkNoTimelineGap() {
    if (pHeader == nullptr)
        return false;

    auto current = pHeader;
    while (true) {
        auto next = pHeader->next;
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

    appendEffectItem(ei, pEnd->prev);

}

void EffectLine::appendPre(Effect *effect) {
    if (effect == nullptr)
        return;

    auto ei = new EffectItem;

    ei->pEffect = effect;

    appendEffectItem(ei, pHeader);
}

void EffectLine::appendEffectItem(EffectItem *item, EffectItem *ei) {
    auto next = ei->next;

    ei->next = item;
    item->prev = ei;

    next->prev = item;
    item->next = next;
}

