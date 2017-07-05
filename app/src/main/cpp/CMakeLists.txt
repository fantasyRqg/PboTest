cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -Wall")


file(GLOB_RECURSE Muses_SRC
            "*.cpp"
            "*.h"
            "*.hpp"
            )

add_library(
             native-lib

             SHARED

             ${Muses_SRC})

find_library(
              log-lib
              log )

target_link_libraries(native-lib

                       android
                       EGL
                       GLESv3
                       ${log-lib} )