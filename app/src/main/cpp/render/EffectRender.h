//
// Created by ranqingguo on 7/4/17.
//

#ifndef PBOTEST_RENDER_H
#define PBOTEST_RENDER_H


#include <GLES3/gl3.h>
#include <android/asset_manager.h>

class EffectRender {
public:


    /**
     * z order , min one draw first
     * @return
     */
    int getZOrder() const;

    void setZOrder(int ZOrder);

    /**
     * assume: after Bpo upload texture via DMA, we can use CPU do something
     *
     */
    virtual void prepareNextFrame() = 0;

    virtual void drawFrame(long millsecond) = 0;

    virtual bool setUp(AAssetManager *amgr) = 0;

    virtual bool tearDown() = 0;

    GLuint loadShader(AAssetManager *amgr, const char *fname, GLenum type);

protected:
    int mZOrder;
};


#endif //PBOTEST_RENDER_H

