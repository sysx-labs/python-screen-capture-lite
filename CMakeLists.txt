cmake_minimum_required(VERSION 3.20.0)

project(python-screen-capture-lite)

#############################################################################################
#####
##### Options
#####
#############################################################################################
set(LIBRARY_TYPE SHARED)

#############################################################################################
#####
##### CMAKE Setup and Includes
#####
#############################################################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Lock this section so that parallel CMake runs won't clash on checkout in the same directory
file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)

# Includes
include(cmake/CPM.cmake)

#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 20)
#set(PYBIND11_CPP_STANDARD /std:c++20) # Enables some MSVC C++17 features
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)


#############################################################################################
#####
##### Options Configration
#####
#############################################################################################

if(IS_PYBIND_BUILD)
    set(LIBRARY_TYPE SHARED)
endif()

if (LIBRARY_TYPE STREQUAL "SHARED")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

if (WIN32)
    set(OPTIMIZATIONS_RELASE /Ox)
    set(OPTIMIZATIONS_DEBUG /O0)
else ()
    set(OPTIMIZATIONS_RELASE -O3 -march=native -Wall)
    set(OPTIMIZATIONS_DEBUG -Wall -O0) # -Og -march=native -Wall -fstack-protector-all
endif ()


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(${OPTIMIZATIONS_RELASE})
else()
    add_compile_options(${OPTIMIZATIONS_DEBUG})
    add_compile_definitions(DEBUG=1)
    add_compile_options(-gdwarf-4)  # This allows per to debug with llvm
endif()

# Also build shared library versions where possible.
set(BUILD_SHARED_LIBS ON)


set(python-screen-capture-lite_DEPENDENCIES
        python3
        pybind11
        opencv
)



#############################################################################################
#####
##### Find Required Packages
#####
#############################################################################################
CPMAddPackage(
        NAME screen_capture_lite
        GIT_REPOSITORY "https://github.com/smasherprog/screen_capture_lite"
        GIT_TAG "master"
        OPTIONS
        "BUILD_EXAMPLE OFF"
        "BUILD_SHARED_LIBS OFF"
)
find_package(Python COMPONENTS Interpreter NumPy REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
#############################################################################################
#####
##### python-screen-capture-lite - build source file variable
#####
#############################################################################################

set(python-screen-capture-lite_SOURCES
    src/WindowCapture.cpp
)

#############################################################################################
#####
##### python-screen-capture-lite - link targets
#####
#############################################################################################
set(python-screen-capture-lite_LINK_TARGETS
        Threads::Threads
        Python::NumPy

        # TODO for windows?
        # https://github.com/smasherprog/screen_capture_lite/blob/master/src_cpp/CMakeLists.txt#L111
        ${X11_LIBRARIES}
        ${X11_Xfixes_LIB}
        ${X11_XTest_LIB}
        ${X11_Xinerama_LIB}
        ${CMAKE_THREAD_LIBS_INIT}


        screen_capture_lite_static
        opencv_core
        opencv_highgui
)


#############################################################################################
#####
##### python-screen-capture-lite - c++ executable
#####
#############################################################################################
if (NOT IS_PYBIND_BUILD)

    add_executable(python-screen-capture-lite_exec src/main.cpp src/WindowCapture.cpp src/WindowCapture.h)
    target_include_directories(
            python-screen-capture-lite_exec
            PUBLIC
            ${screen_capture_lite_SOURCE_DIR}/include
    )
    target_link_libraries(
            python-screen-capture-lite_exec
            PUBLIC
            ${python-screen-capture-lite_LINK_TARGETS}
    )
endif ()


#############################################################################################
#####
##### PYBIND11 Builds
#####
#############################################################################################
set(python-screen-capture-lite-bind-files
        src/pybind/converters/ndarray_converter.cpp
        src/pybind/ScreenCaptureLite.cpp
        src/pybind/WindowCapture.cpp
)
pybind11_add_module(screen_capture_lite ${python-screen-capture-lite-bind-files} ${python-screen-capture-lite_SOURCES})
target_link_libraries(screen_capture_lite
        PUBLIC
        ${python-screen-capture-lite_LINK_TARGETS}
)
target_include_directories(
        screen_capture_lite
        PUBLIC
        ${screen_capture_lite_SOURCE_DIR}/include
)
