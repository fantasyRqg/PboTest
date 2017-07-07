//
// Created by ranqingguo on 7/5/17.
//
#include <cstdlib>
#include <ctime>


#include "../util/common.h"
#include "PboTestRender.h"


#undef TAG
#define TAG "PboTestRender"

void PboTestRender::prepareNextFrame() {

}


double randDouble() {
    return (double) std::rand() / (RAND_MAX);
}

/**
* Fill the buffer with a regular pattern.  This should compress well.
*/
void patternPixelSource(unsigned char *buf, size_t len, int width, int height, int index) {
    unsigned char *array = buf;

    std::srand((unsigned int) std::time(0));


    // generate 4 random RGBA colors
    unsigned char colors[4][4];
    for (int i = 0; i < 4; i++) {
        colors[i][0] = (unsigned char) (256 * randDouble() - 128);
        colors[i][1] = (unsigned char) (256 * randDouble() - 128);
        colors[i][2] = (unsigned char) (256 * randDouble() - 128);
        colors[i][3] = 255; //(byte) (256 * Math.random() - 128);
    }

    int repCount = (index % 4) + 1;
    int off = 0;
    for (int y = 0; y < height; y++) {
        int colIndex = (y / repCount) % 4;
        for (int x = 0; x < width;) {
            // repeat the color N times (if possible)
            for (int rep = 0; rep < repCount && x < width; rep++, x++) {
                // copy the Nth color to the current pixel
                array[off++] = colors[colIndex][0];
                array[off++] = colors[colIndex][1];
                array[off++] = colors[colIndex][2];
                array[off++] = colors[colIndex][3];
            }
            colIndex = (colIndex + 1) % 4;
        }
    }

}


void PboTestRender::drawFrame(long millsecond) {
    using namespace glCommon;

    glBindVertexArray(mVertexArrayId);


    int width = 3840;
    int height = 2160;
//
//    int width = 1920;
//    int height = 1080;
    size_t imageSize = (size_t) (width * height * 4);

    GLuint pbo[10];
    glGenBuffers(10, pbo);


    for (int i = 0; i < 10; ++i) {
        createBpo(imageSize, pbo, i);
    }

    GLuint textureId[10];
    glGenTextures(10, textureId);

    for (int i = 0; i < 10; ++i) {
        glBindTexture(GL_TEXTURE_2D, textureId[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

    }

    unsigned char *imageArray[10];

    for (int i = 0; i < 10; ++i) {
        imageArray[i] = (unsigned char *) malloc(imageSize);

        patternPixelSource(imageArray[i], imageSize, width, height, 0);
    }

    for (int j = 0; j < 10; ++j) {
        for (int i = 0; i < 10; ++i) {
            auto lastTime = systemnanotime();

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[i]);
            glBindTexture(GL_TEXTURE_2D, textureId[i]);

//            glBufferData(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr) imageSize, imageArray[i],
//                         GL_STREAM_DRAW);
            auto pboBufferPtr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, (GLsizeiptr) imageSize,
                                                 GL_MAP_WRITE_BIT);

            memcpy(pboBufferPtr, imageArray[(i + j) % 10], imageSize);

            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//                         0);

//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//        auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
//
//        glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
//        glDeleteSync(sync);


            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            auto current = systemnanotime();
            LOGD("iter %d , cost %lld", i, current - lastTime);
        }

//        glFlush();
//        glFinish();

        for (int i = 0; i < 10; ++i) {
            auto start = systemnanotime();
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            LOGV("glTexSubImage2D cost %10lld", systemnanotime() - start);
        }


        auto start = systemnanotime();
        glFlush();
        glFinish();

        LOGI("finish cost = %10lld", systemnanotime() - start);
    }

//    LOGI("with out pbo");
//
//
//    for (int j = 0; j < 10; ++j) {
//
//
//        for (int i = 0; i < 10; ++i) {
//
//            auto lastTime = systemnanotime();
//
//
//            glBindTexture(GL_TEXTURE_2D, textureId[i]);
////            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
////                         imageArray[i]);
//
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
//                            imageArray[(i + j) % 10]);
//
////        auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
////
////        glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
////        glDeleteSync(sync);
//
//
//            glBindTexture(GL_TEXTURE_2D, 0);
//            auto current = systemnanotime();
//            LOGD("without iter %d , cost %10lld", i, current - lastTime);
//
//        }
//
//        auto start = systemnanotime();
//        glFlush();
//        glFinish();
//
//        LOGI("without finish cost = %10lld", systemnanotime() - start);
//    }


    for (int i = 0; i < 10; ++i) {
        free(imageArray[i]);
    }
}

void PboTestRender::createBpo(size_t imageSize, const GLuint *pbo, int index) const {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[index]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr) imageSize, NULL, GL_STREAM_DRAW);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
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