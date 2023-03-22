cmake_minimum_required(VERSION 3.16)

project(implot)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

include_directories(../implot)

set (IMPLOT_SRC 
    ../implot/implot_demo.cpp
    ../implot/implot.cpp
    ../implot/implot.h
    ../implot/implot_internal.h
    ../implot/implot_items.cpp
)

add_library(implot STATIC ${IMPLOT_SRC})

target_include_directories(implot PUBLIC ../imgui)
target_include_directories(implot PUBLIC ../implot)
