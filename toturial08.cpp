//
// Created by peng on 17-9-19.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glShader.h"


int main()
{
    glfwSetErrorCallback([](int errorCode,const char *errorMsg){
        std::cerr << "[" << errorCode << "] " << errorMsg << std::endl;
    });
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize glfw !" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1024,768,"Toturial08-BasicShading",nullptr,nullptr);
    if(!window)
    {
        std::cerr << "glfwCreateWindow Failed !" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if(glewInit())
    {
        std::cerr << "glewInit failed !" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    while(!glfwWindowShouldClose(window) && glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS){
        glfwPollEvents();

        //TODO: do something at here.


        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}