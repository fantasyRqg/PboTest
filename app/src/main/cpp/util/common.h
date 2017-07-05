//
// Created by ranqingguo on 3/9/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_COMMON_H
#define ANDROIDOPENGLEXAMPLES_COMMON_H

#include <GLES3/gl3.h>
#include <android/log.h>


#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)


namespace glCommon {
    bool checkShaderAndPrint(GLuint shader);

    bool checkProgramAndPrint(GLuint program);

    bool checkGlError(const char *funcName);

}


#endif //ANDROIDOPENGLEXAMPLES_COMMON_H
