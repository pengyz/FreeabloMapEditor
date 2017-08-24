cmake_minimum_required(VERSION 3.8)
project(FreeabloMapEditor)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
    message(Error "OpenGL not found")
endif(NOT ZLIB_FOUND)

find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)


include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIR}
        "${PROJECT_SOURCE_DIR}/extern/StormLib/src/"
)


# allow the user to specify extra include dirs and defines on the command line
if(CLI_INCLUDE_DIRS)
    include_directories(${CLI_INCLUDE_DIRS})
endif()
if(CLI_DEFINES)
    add_definitions(${CLI_DEFINES})
endif()


add_subdirectory (extern)

link_libraries(StormLib)

link_libraries(
        ${PNG_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${GLEW_LIBRARIES}
        StormLib
        nuklear
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})



set(SOURCE_FILES
        main.cpp
        nuklear_glfw_gl3.h
        nuklear_glfw_gl3.cpp
        )
add_executable(FreeabloMapEditor ${SOURCE_FILES})