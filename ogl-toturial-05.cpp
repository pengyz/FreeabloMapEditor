#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glShader.h"
#include <iostream>
#include <string>
#include "glShader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// the struct to hold all values we have used.
// just use glfwSetWindowUserPointer to bind it to the GLFWwindow pointer.
struct window_info{
    int width;
    int height;
    std::string title;
    GLuint vertexArrayId;
    GLuint vertexbuffer;            //identify for vertexBuffer
    GLuint colorbuffer;
    GLuint programId;
    GLint matrixId;
    glm::mat4 mvpMatrix;
};

void drawFunc(GLFWwindow *window){
    window_info* info = static_cast<window_info*>(glfwGetWindowUserPointer(window));
    if(!info){
        std::cerr << "drawFunc: get info failed !";
        return;
    }

    glfwGetWindowSize(window, &info->width, &info->height);
    glViewport(0,0,info->width,info->height);
    glUseProgram(info->programId);
//    glUniformMatrix4fv(info->matrixId, 1, GL_FALSE, &info->mvpMatrix[0][0]);
    glBindVertexArray(info->vertexArrayId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES,0, 12 * 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void initFunc(GLFWwindow *window)
{
    window_info *info = static_cast<window_info*>(glfwGetWindowUserPointer(window));
    if(!info){
        std::cerr << "initFunc: get info failed !" << std::endl;
        return;
    }
    const static GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end

            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end

            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,

            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,

            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,

            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,

            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,

            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,

            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static GLfloat g_color_buffer_data[12 *3 * 3] = {0};


    srand(time(NULL));
    for (int i = 0; i < sizeof(g_color_buffer_data) / sizeof(GLfloat); ++i) {
        g_color_buffer_data[i] = GLfloat(rand() % 100) / 100.0f;
    }

    glGenVertexArrays(1,&info->vertexArrayId);
    glBindVertexArray(info->vertexArrayId);
    //gen buffer
    glGenBuffers(1, &info->vertexbuffer); //gen 1 buffer,saved in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, info->vertexbuffer);    //bind the generated buffer, otherwise the following operation will take effect on it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
            0,//same as the bind location
            3,//size of the array element, just 3 float makes a point.
            GL_FLOAT,//we use float for point
            GL_FALSE,//not normalize
            0,//stride
            (void*)0//array buffer offset.
    );
    glGenBuffers(1, &info->colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, info->colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glBindVertexArray(0);
    info->programId = Misc::loadShader("../resources/shaders/SimpleVertexShader.glsl","../resources/shaders/SimpleFragmentShader.glsl");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(info->programId);
    //let's create the MVP matrix!
    glm::mat4 projectionMartrix = glm::perspective(glm::radians(45.0f),(float)info->width / (float)info->height,0.1f, 100.0f);
//    glm::mat4 projectionMartrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(4,3,3),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
    );
    glm::mat4 modelMatrix = glm::mat4( 1.0f);
    //wtf? clion give the wrong warning...
    info->mvpMatrix = projectionMartrix * viewMatrix * modelMatrix;
    info->matrixId = glGetUniformLocation(info->programId, "MVP");
    //set uniform martix, for the static draw, we just need to translate it once.
    //shader has the global state. it can 'remember' the value we already pasted.
    //if need dynamic effect, caculate and transfer it due peer frame drawing.
    glUniformMatrix4fv(info->matrixId, 1, GL_FALSE, &info->mvpMatrix[0][0]);
    std::cout << "initFunc success !" << std::endl;
}

void cleanFunc(GLFWwindow *window){
    window_info *info = static_cast<window_info*>(glfwGetWindowUserPointer(window));
    if(!info){
        std::cerr << "cleanFun: get info failed !" << std::endl;
        return;
    }
    glDeleteBuffers(1,&info->vertexbuffer);
    glDeleteBuffers(1, &info->colorbuffer);
    glDeleteVertexArrays(1,&info->vertexArrayId);
    glDeleteProgram(info->programId);
    //delete info
    glfwSetWindowUserPointer(window, nullptr);
    delete info;
    //destory window
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    std::cout << "cleanFunc success !" << std::endl;
}

int main(int argc, const char** argv){

    glfwSetErrorCallback([](int errorCode, const char* errorMsg){
        std::cerr << "[" << errorCode << "] " << errorMsg <<  std::endl;
    });
    if(!glfwInit()){
        std::cerr << "glfwInit failed !" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_info *info = new window_info;
    info->width = 800;
    info->height = 600;
    info->title = "ogl-toturial-texture-clube";
    GLFWwindow *window = glfwCreateWindow(info->width,info->height,info->title.c_str(),nullptr, nullptr);
    if(!window){
        delete info;
        std::cout << "glfwCreateWindow failed !" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(window,info);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit()){
        glfwTerminate();
        std::cerr << "glewInit failed !" << std::endl;
        return -1;
    }
    //初始化
    initFunc(window);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //绘制
        drawFunc(window);
        //swap buffer
        glfwSwapBuffers(window);
    }
    //清理
    cleanFunc(window);
    glfwTerminate();
    return 0;
}