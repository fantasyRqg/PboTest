//
// Created by ranqingguo on 7/4/17.
//

#include <sstream>

#include "EglCore.h"
#include "../util/LogUtil.h"

#undef TAG
#define TAG "EglCore"

#define EGL_RESULT_CHECK(X) do { \
                                   EGLint error = eglGetError(); \
                                   if (!(X) || error != EGL_SUCCESS) { \
                                       __android_log_print(ANDROID_LOG_ERROR, TAG, \
                                          "EGL error '%d' at %s:%d", error, __FILE__, __LINE__);\
                                       return false; \
                                    } \
                            } while (0)


EglCore::EglCore()
        : mEglDisplay(EGL_NO_DISPLAY), mEglContext(EGL_NO_CONTEXT) {

}

EglCore::~EGLWrapper() {
    tearDown();
}

bool EglCore::setUp() {
    static const EGLint DEFAULT_CONTEXT_ATTRIBS[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE};

    static const EGLint DEFAULT_CONFIG_ATTRIBS[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,


#ifdef EGL_KHR_create_context
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
#else
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif

            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 0,
            EGL_NONE};

    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGL_RESULT_CHECK(mEglDisplay != EGL_NO_DISPLAY);

    EGLint major;
    EGLint minor;
    EGL_RESULT_CHECK(eglInitialize(mEglDisplay, &major, &minor));

    EGLint numConfigs = 0;
    EGL_RESULT_CHECK(
            eglChooseConfig(mEglDisplay, DEFAULT_CONFIG_ATTRIBS, &mGlConfig, 1, &numConfigs)
            && (numConfigs > 0));

//    mEglSurface = eglCreateWindowSurface(mEglDisplay, mGlConfig, mWindow, NULL);
//    EGL_RESULT_CHECK(mEglSurface != EGL_NO_SURFACE);

    mEglContext = eglCreateContext(mEglDisplay, mGlConfig, EGL_NO_CONTEXT, DEFAULT_CONTEXT_ATTRIBS);
    EGL_RESULT_CHECK(mEglContext != EGL_NO_CONTEXT);

//    EGL_RESULT_CHECK(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext));


    LOGI("%s", glGetString(GL_VERSION));
    LOGI("%s", glGetString(GL_VENDOR));
    LOGI("%s", glGetString(GL_RENDERER));
    LOGI("%s", glGetString(GL_EXTENSIONS));
    return true;
}

bool EglCore::tearDown() {
    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);


    if (mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
    }

    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglTerminate(mEglDisplay);
    }


    mEglContext = EGL_NO_CONTEXT;
    mEglDisplay = EGL_NO_DISPLAY;

    ANativeWindow_release(mWindow);
    return true;
}


void EglCore::releaseSurface(EGLSurface eglSurface) {
    eglDestroySurface(mEglDisplay, eglSurface);
}

EGLSurface EglCore::createWindowSurface(NativeWindowType window) {

    EGLint attrib_list[] = {
            EGL_NONE
    };

    EGLSurface eglSurface = eglCreateWindowSurface(mEglDisplay, mGlConfig, window, attrib_list);

    checkEglError("createWindowSurface");


    if (eglSurface == nullptr) {
        throw std::runtime_error("createWindowSurface failure , surface is null");
    }

    return eglSurface;
}

EGLSurface EglCore::createOffscreenSurface(int width, int height) {
    return nullptr;
}

void EglCore::makeCurrent(EGLSurface eglSurface) {
    if (mEglDisplay == EGL_NO_DISPLAY) {
        // called makeCurrent() before create?
        LOGD(TAG, "NOTE: makeCurrent w/o display");
    }
    if (!eglMakeCurrent(mEglDisplay, eglSurface, eglSurface, mEglContext)) {
        throw new std::runtime_error("eglMakeCurrent failed");
    }
}

bool EglCore::swapBuffers(EGLSurface eglSurface) {
    return eglSwapBuffers(mEglDisplay, eglSurface) == EGL_TRUE;
}

void EglCore::checkEglError(std::string msg) {
    int error;
    if ((error = eglGetError()) != EGL_SUCCESS) {

        std::stringstream ss;
        ss << msg << ": EGL error: 0x" << std::hex << error;
        throw std::runtime_error(ss.str());
    }
}

int EglCore::querySurface(EGLSurface eglSurface, int what) {
    int value[1];
    eglQuerySurface(mEglDisplay, eglSurface, what, value);

    return value[0];
}


void EglCore::presentationTimeANDROID(EGLSurface eglSurface, long nanoseconds) {
    eglPresentationTimeANDROID(mEglDisplay, eglSurface, nanoseconds);
}
