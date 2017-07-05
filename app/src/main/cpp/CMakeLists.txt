cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -Wall")

add_library(
             native-lib

             SHARED

             native-lib.cpp
             gl/EglCore.cpp
             gl/surface/EglSurfaceBase.cpp
             gl/surface/OffscreenSurface.cpp
             gl/surface/WindowSurface.cpp


             RenderFrame.cpp
             framesource/IFrameSource.cpp


             PboPool.cpp
             render/EffectRender.cpp
             EffectStage.cpp
              )

find_library(
              log-lib
              log )

target_link_libraries(native-lib

                       android
                       EGL
                       GLESv3
                       ${log-lib} )