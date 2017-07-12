//
// Created by ranqingguo on 7/4/17.
//

#ifndef MUSES_RENDER_H
#define MUSES_RENDER_H

//#ifndef GL_GLEXT_PROTOTYPES
//#define GL_GLEXT_PROTOTYPES
//#endif //GL_GLEXT_PROTOTYPES

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include "../gl/surface/EglSurfaceBase.h"

struct PboRes;


/**
 * all gl operation run
 */
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


    virtual GLuint getTextureIdAt(int index) = 0;

    virtual int getTextureCount() = 0;

    /**
     * make sure pbo is ready ,by check pbo sync
     * @param timeUs draw time us
     */
    virtual void drawFrame(int64_t timeUs) = 0;

    virtual bool setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) = 0;

    virtual bool tearDown() = 0;

    GLuint loadShader(AAssetManager *amgr, const char *fname, GLenum type);

//    void makeSurePboUploaded();

    bool isRenderFbo() const;

    void setRenderFbo(bool renderFbo);


protected:
    int mZOrder;
    bool mRenderFbo = false;
};


#endif //MUSES_RENDER_H

