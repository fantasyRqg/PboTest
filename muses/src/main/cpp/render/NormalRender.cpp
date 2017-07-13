//
// Created by ranqingguo on 7/13/17.
//

#include "NormalRender.h"
#include "../util/common.h"


int NormalRender::getTextureCount() {

    return TEXTURE_COUNT;
}

void NormalRender::drawFrame(int64_t timeUs) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *) (sizeof(GLfloat) * 3 * 4));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureIds[0]);
    glUniform1i(mVideoTexLoc, 0);

    auto mvp = glm::mat4(1.0f);
    glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, &mvp[0][0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
}

bool NormalRender::setUp(AAssetManager *amgr, EglSurfaceBase *eglSurface) {
    static const GLfloat vertex_data[] = {
            //big
            -1, -1, 0,
            1, -1, 0,
            1, 1, 0,
            -1, 1, 0,

            1, 1,
            0, 1,
            0, 0,
            1, 0,

    };


    static GLubyte indicates[] = {
            0, 1, 3, 2
    };

    auto vs = loadShader(amgr, "normal.vs.glsl", GL_VERTEX_SHADER);
    auto fs = loadShader(amgr, "normal.fs.glsl", GL_FRAGMENT_SHADER);

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

    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicates), indicates, GL_STATIC_DRAW);

    glGenTextures(TEXTURE_COUNT, mTextureIds);


    mMvpLocation = glGetUniformLocation(mProgram, "mvpMat");
    mVideoTexLoc = glGetUniformLocation(mProgram, "videoTex");

    return true;
}

bool NormalRender::tearDown() {
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteProgram(mProgram);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mIbo);
    glDeleteTextures(1, mTextureIds);
    return false;
}

GLuint *NormalRender::getTextureIdArray() {
    return mTextureIds;
}


