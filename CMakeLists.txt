cmake_minimum_required(VERSION 3.8)

include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.19.225.tar.gz"
  SHA1 "96acb7ba4f1023018d271d8b53f229c5d3eff9e7"  
  LOCAL
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE debug)
project(FreeabloMapEditor)

set(CMAKE_CXX_STANDARD 17)

set(HUNTER_LIBS "")
hunter_add_package(glfw)
find_package(glfw3 CONFIG REQUIRED)
set(HUNTER_LIBS ${HUNTER_LIBS} glfw)

hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
set(HUNTER_LIBS ${HUNTER_LIBS} glew::glew)

hunter_add_package(stormlib)
find_package(stormlib CONFIG REQUIRED)
set(HUNTER_LIBS ${HUNTER_LIBS} stormlib::stormlib)

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
set(HUNTER_LIBS ${HUNTER_LIBS} glm)

hunter_add_package(Boost COMPONENTS filesystem system regex)
find_package(Boost CONFIG REQUIRED  filesystem system regex)
set(HUNTER_BOOST_LIBS Boost::filesystem Boost::system Boost::regex)
set(HUNTER_LIBS ${HUNTER_LIBS} ${HUNTER_BOOST_LIBS})

find_package(OpenGL REQUIRED)


include_directories(
        ${OPENGL_INCLUDE_DIRS}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
message("Cmake binary dir:" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory(extern/NuklearLib)

# FreeabloMapEditor
add_executable(FreeabloMapEditor
        main.cpp
        nuklear_glfw_gl3.h
        nuklear_glfw_gl3.cpp
        glShader.cpp glShader.h)
target_link_libraries( FreeabloMapEditor
        ${HUNTER_LIBS}
        nuklear
)


# #ogl-toturial05
# add_executable(toturial05
#         glShader.cpp
#         glShader.h
#         toturial05.cpp)

# target_link_libraries(toturial05
#         ${HUNTER_LIBS})


# #ogl-toturial08
# add_executable(toturial08
#         glShader.cpp
#         glShader.h
#         toturial08.cpp)

# target_link_libraries(toturial08
#         ${HUNTER_LIBS})
