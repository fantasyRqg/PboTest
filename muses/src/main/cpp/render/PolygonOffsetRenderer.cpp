//
// Created by ranqingguo on 3/18/17.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


#include "PolygonOffsetRenderer.h"
#include "../util/common.h"
#include "../Uploader.h"


void PolygonOffsetRenderer::prepareDrawFrame() {

}


int PolygonOffsetRenderer::getNeededPboCount() {
    return TEXTURE_COUNT;
}

void PolygonOffsetRenderer::drawFrame(int64_t timeUs) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);


    for (int i = 0; i < TEXTURE_COUNT; ++i) {
        auto pbores = mPboResArray[i];
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbores->pbo);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureId[i]);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glCommon::checkGlError("bind texture and pbo");

    }

    glUseProgram(mProgram);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glCommon::checkGlError("glBindBuffer");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (const void *) (sizeof(GLfloat) * 3 * 4 * 2));
    glCommon::checkGlError("glVertexAttribPointer");

    glm::mat4 mvp = mProjMatrix * mViewMatrix *
                    glm::rotate(glm::mat4(1.0f), glm::radians(float(timeUs / 1000L) * 0.1f),
                                glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, &mvp[0][0]);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicatesVbo[1]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
    glCommon::checkGlError("glDrawElements");

//    glDepthFunc(GL_LEQUAL);
//
//
//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset(-1.0f, -2.0f);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicatesVbo[0]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
    glCommon::checkGlError("glDrawElements 2");

}

bool PolygonOffsetRenderer::setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) {
#define GRAY_COLOR 0.9f,0.5f,0.3f
#define WHITE_COLOR 1.0f,1.0f,1.0f

    static const GLfloat vertex_data[] = {

            //big
            -0.5f, -0.5f, 0,
            0.5f, -0.5f, 0,
            0.5f, 0.5f, 0,
            -0.5f, 0.5f, 0,
//small
            -0.25f, -0.25f, 0,
            0.25f, -0.25f, 0,
            0.25f, 0.25f, 0,
            -0.25f, 0.25f, 0,

            WHITE_COLOR,
            WHITE_COLOR,
            WHITE_COLOR,
            WHITE_COLOR,
            GRAY_COLOR,
            GRAY_COLOR,
            GRAY_COLOR,
            GRAY_COLOR,

    };


    static GLubyte small[] = {
            4, 5, 7, 6

    };
    static GLubyte big[] = {
            0, 1, 3, 2
    };


    GLuint vs = loadShader(amgr, "polygon.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader(amgr, "polygon.fs.glsl", GL_FRAGMENT_SHADER);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vs);
    glAttachShader(mProgram, fs);
    glLinkProgram(mProgram);
    glCommon::checkProgramAndPrint(mProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);


    glGenBuffers(2, mIndicatesVbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicatesVbo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(small), small, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicatesVbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(big), big, GL_STATIC_DRAW);


    mMvpLocation = glGetUniformLocation(mProgram, "mvpMat");


    mProjMatrix = glm::perspective(45.0f, eglSurface->getAspect(), 1.0f, 100.0f);
    mViewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glGenTextures(TEXTURE_COUNT, mTextureId);

//    LOGV("render set up");
//    glCommon::checkGlError("set up");
    return true;
}

bool PolygonOffsetRenderer::tearDown() {
    glDeleteProgram(mProgram);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(2, &mIndicatesVbo[0]);
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteTextures(TEXTURE_COUNT, mTextureId);
    return true;
}
