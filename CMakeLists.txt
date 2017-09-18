cmake_minimum_required(VERSION 3.8)
project(FreeabloMapEditor)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(PNG REQUIRED)
if(NOT PNG_FOUND)
    message("PNG not found")
endif(NOT PNG_FOUND)

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(Error "OpenGL not found")
endif(NOT OPENGL_FOUND)



# allow the user to specify extra include dirs and defines on the command line
if(CLI_INCLUDE_DIRS)
    include_directories(${CLI_INCLUDE_DIRS})
endif()
if(CLI_DEFINES)
    add_definitions(${CLI_DEFINES})
endif()


add_subdirectory (extern)

link_libraries(StormLib)
link_libraries(glm)



include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIR}
)



set(COMMON_LIBS
        ${OPENGL_LIBRARY}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${PNG_LIBRARY}
        glew
        glfw
        )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})



# FreeabloMapEditor
add_executable(FreeabloMapEditor
        main.cpp
        nuklear_glfw_gl3.h
        nuklear_glfw_gl3.cpp
        glShader.cpp glShader.h)
target_link_libraries( FreeabloMapEditor
        ${COMMON_LIBS}
        StormLib
        nuklear
)

#ogl-toturial05
add_executable(toturial05
        glShader.cpp
        glShader.h
        toturial05.cpp)

target_link_libraries(toturial05
        ${COMMON_LIBS})


#ogl-toturial08
add_executable(toturial08
        glShader.cpp
        glShader.h
        toturial08.cpp)

target_link_libraries(toturial08
        ${COMMON_LIBS})