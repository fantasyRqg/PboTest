cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -Wall ")

add_definitions(-DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES)


set(EXTERNAL_DIR ../../../../external)

file(GLOB_RECURSE Muses_SRC
            "*.cpp"
            #"*.h"
            #"*.hpp"
            )

set(Lib_Name "native-lib")

add_library(
             ${Lib_Name}

             SHARED

             ${Muses_SRC})

find_library(
              log-lib
              log )

target_include_directories(${Lib_Name} PRIVATE
        ${EXTERNAL_DIR}/glm
        )

target_link_libraries(${Lib_Name}

                       mediandk
                       android
                       EGL
                       GLESv3
                       ${log-lib} )