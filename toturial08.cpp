//
// Created by peng on 17-9-19.
//

#include <iostream>

#include "glShader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
  glfwSetErrorCallback([](int errorCode, const char *errorMsg) {
    std::cerr << "[" << errorCode << "] " << errorMsg << std::endl;
  });
  if (!glfwInit()) {
    std::cerr << "Failed to initialize glfw !" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Toturial08-BasicShading", nullptr, nullptr);
  if (!window) {
    std::cerr << "glfwCreateWindow Failed !" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if (glewInit()) {
    std::cerr << "glewInit failed !" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwPollEvents();
  // set the mouse center the window widget
  int winW, winH;
  glfwGetWindowSize(window, &winW, &winH);
  glfwSetCursorPos(window, winW / 2, winH / 2);

  glClearColor(0.1f, 0.3f, 0.7f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  GLuint glVertexArrayId;
  glGenVertexArrays(1, &glVertexArrayId);
  glBindVertexArray(glVertexArrayId);

  GLuint glProgramId =
      Misc::loadShader("../resources/shaders/StandardShadingVertex.glsl",
                       "../resources/shaders/StandardShadingFrag.glsl");

  // Get a handle for "MVP" uniform
  GLuint MatrixId = glGetUniformLocation(glProgramId, "MVP");
  GLuint ViewMatrixId = glGetUniformLocation(glProgramId, "V");
  GLuint ModelMatrixId = glGetUniformLocation(glProgramId, "M");

  // load the texture
  GLuint Texture = 0;


  while (!glfwWindowShouldClose(window) &&
         glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
    glfwPollEvents();
    // TODO: do something at here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
