//
// Created by ranqingguo on 7/4/17.
//

#ifndef MUSES_RENDER_H
#define MUSES_RENDER_H


#include <GLES3/gl3.h>
#include <android/asset_manager.h>

struct PboRes;

class Render {
public:


    /**
     * z order , min one draw first
     * @return
     */
    int getZOrder() const;

    void setZOrder(int ZOrder);

    /**
     * assume: after Bpo upload texture via DMA, we can use CPU do something
     * run on Painter
     */
    virtual void prepareDrawFrame() = 0;

    /**
     * 填入正在上传资源
     * @param pbos
     */
    virtual void setDrawPboRes(PboRes **pboReses, int len) =0;

    virtual int getNeededPboCount() = 0;

    /**
     * make sure pbo is ready ,by check pbo sync
     * @param timeUs draw time us
     */
    virtual void drawFrame(long timeUs) = 0;

    virtual bool setUp(AAssetManager *amgr) = 0;

    virtual bool tearDown() = 0;

    GLuint loadShader(AAssetManager *amgr, const char *fname, GLenum type);

    bool isRenderFbo() const;

    void setRenderFbo(bool renderFbo);

protected:
    int mZOrder;
    bool mRenderFbo = false;
};


#endif //MUSES_RENDER_H

