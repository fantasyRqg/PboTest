//
// Created by ranqingguo on 3/9/17.
//

#include <stdlib.h>
#include <linux/time.h>
#include <time.h>

#include "common.h"


namespace glCommon {

#undef TAG
#define TAG "glCommon"

    bool checkShaderAndPrint(GLuint shader) {

        GLint compiled = GL_FALSE;
        // Check the compile status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            GLint infoLen = 0;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);

                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                LOGE("Error compiling shader:\n%s\n", infoLog);
                free(infoLog);
            }

            glDeleteShader(shader);
            return false;
        }

        return true;
    }

    bool checkProgramAndPrint(GLuint program) {
        GLint linked;
        // Check the link status
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(program, infoLen, NULL, infoLog);
                LOGE("Error linking program:\n%s\n", infoLog);
                free(infoLog);
            }
            glDeleteProgram(program);
            return false;
        }

        return true;
    }


    const char *getGlErrorStr(GLint e) {
        switch (e) {
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION";
//            case GL_INVALID_INDEX:
//                return "GL_INVALID_INDEX";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE";

            default:
                return "UNKNOWN_ERROR";
        }
    }

    bool checkGlError(const char *funcName) {
        GLint err = glGetError();
        if (err != GL_NO_ERROR) {
            LOGE("GL error after %s(): %s\n", funcName, getGlErrorStr(err));
            return true;
        }
        return false;
    }



    int64_t systemnanotime() {
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000LL + now.tv_nsec;
    }


#undef TAG
}
