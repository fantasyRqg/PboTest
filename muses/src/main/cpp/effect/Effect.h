//
// Created by ranqingguo on 7/7/17.
//

#ifndef PBOTEST_VIDEOEFFECT_H
#define PBOTEST_VIDEOEFFECT_H

/**
 * 描述需要的素材情况，并组装相应的绘制工具
 *
 * describe framesource info which use to draw frame
 * and assemble draw classes
 */
class Effect {
public:


    Effect(long startTimeMills, long endTimeMills);


    /**
     * start time == duration
     * end time = durationMills
     * @param durationMills
     */
    Effect(long durationMills);

    long getStartTimeMills() const;

    long getEndTimeMills() const;

    bool offsetTime(long offsetMills);

private:
    long mStartTimeMills;
    long mEndTimeMills;

};


#endif //PBOTEST_VIDEOEFFECT_H
