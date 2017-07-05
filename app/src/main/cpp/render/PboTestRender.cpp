//
// Created by ranqingguo on 7/5/17.
//
#include <cstdlib>

#include "PboTestRender.h"


void PboTestRender::prepareNextFrame() {

}

/**
        * Fill the buffer with a regular pattern.  This should compress well.
        */
void patternPixelSource(unsigned char *buf, int len, int index) {
    unsigned char *array = buf;

    // generate 4 random RGBA colors
    unsigned char colors[4][4];
    for (int i = 0; i < 4; i++) {
        colors[i][0] = (256 * Math.random() - 128);
        colors[i][1] = (256 * Math.random() - 128);
        colors[i][2] = (256 * Math.random() - 128);
        colors[i][3] = 255; //(byte) (256 * Math.random() - 128);
    }

    int repCount = (index % 4) + 1;
    int off = 0;
    for (int y = 0; y < mHeight; y++) {
        int colIndex = (y / repCount) % 4;
        for (int x = 0; x < mWidth;) {
            // repeat the color N times (if possible)
            for (int rep = 0; rep < repCount && x < mWidth; rep++, x++) {
                // copy the Nth color to the current pixel
                array[off++] = colors[colIndex][0];
                array[off++] = colors[colIndex][1];
                array[off++] = colors[colIndex][2];
                array[off++] = colors[colIndex][3];
            }
            colIndex = (colIndex + 1) % 4;
        }
    }

    if (false) saveTestBitmap(buf, index);
}

void PboTestRender::drawFrame(long millsecond) {

    glBindVertexArray(mVertexArrayId);

    int imageSize = 1920 * 1080 * 4;

    GLuint pbo;
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, imageSize, NULL, GL_STREAM_DRAW);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);

    unsigned char *iamge = (unsigned char *) malloc(imageSize);

}

bool PboTestRender::setUp(AAssetManager *amgr) {
    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);




//    mProgramId = glCreateProgram();
//    GLuint fShader = loadShader(amgr, "pbotest.fs.glsl", GL_FRAGMENT_SHADER);
//    GLuint vShader = loadShader(amgr, "pbotest.vs.glsl", GL_VERTEX_SHADER);
//
//    glAttachShader(mProgramId, fShader);
//    glAttachShader(mProgramId, vShader);
//
//    glLinkProgram(mProgramId);
//    glValidateProgram(mProgramId);

    return true;
}

bool PboTestRender::tearDown() {
    return true;
}
