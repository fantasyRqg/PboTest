//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_RENDER_H
#define PBOTEST_RENDER_H


class EffectRender {
public:
    /**
     * z order , min one draw first
     * @return
     */
    int getZOrder() const;

    void prepareNextFrame();

    virtual void drawFrame() = 0;

    virtual bool setUp() = 0;

    virtual bool tearDown() = 0;


protected:
    int mZOrder;
};


#endif //PBOTEST_RENDER_H

