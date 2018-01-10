#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "fa_nuklear.h"
#include "nuklear_glfw_gl3.h"
#include "glShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <png.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void drawMainGui(GLFWwindow *window)
{
    //let's draw a simple ui.
    nk_context *ctx =
        static_cast<nk_context *>(glfwGetWindowUserPointer(window));
    int width = 0,height = 0;
    glfwGetWindowSize(window,&width, &height);
    if(!ctx) return;
    if(nk_begin(ctx, u8"Кнопка", nk_rect(0,0,width,height / 4 < 200 ? 200 : height / 4),
                NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)){
        //draw it
        nk_layout_row_dynamic(ctx,30,2);
        if(nk_button_label(ctx,"选择mpq文件"))
        {
            std::cout << "mpq label clicked !" << std::endl;
        }
        if(nk_button_text(ctx,"nk_button_text",16))
        {
            std::cout << "nk_button_text" << std::endl;
        }
    }
    nk_end(ctx);
    nk_glfw3_render(NK_ANTI_ALIASING_ON,MAX_VERTEX_BUFFER,MAX_ELEMENT_BUFFER);
}


int main() {
    //ok,let's init glfw and create a window
    glfwSetErrorCallback([](int errorCode,const char* errorMsg){
            std::cerr << "[" << errorCode << "] " << errorMsg <<  std::endl;
    });
    if(!glfwInit())
    {
        std::cerr << "glfwInit failed !" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800,600,"测试窗口",nullptr,nullptr);
    if(!window)
    {
        std::cout << "glfwCreateWindow failed !" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    int width = 0,height = 0;
    glfwGetWindowSize(window,&width,&height);
    glViewport(0,0,width,height);
    glewExperimental = GL_TRUE;
    if(GLEW_OK!= glewInit())
    {
        std::cout << "glewInit failed !" << std::endl;
        return -1;
    }

    struct nk_context *ctx = nullptr;
    ctx = nk_glfw3_init(window,NK_GLFW3_INSTALL_CALLBACKS);
    glfwSetWindowUserPointer(window, ctx);
    //font baking
    struct nk_font_atlas *atlas = nullptr;
    struct nk_font_config config = nk_font_config(14);
    config.oversample_h = 1;
    config.oversample_v = 1;
//    config.range = nk_font_cyrillic_glyph_ranges();   //this should success with text Кнопка
    config.range = nk_font_chinese_glyph_ranges();
    nk_glfw3_font_stash_begin(&atlas);
    struct nk_font *font = nk_font_atlas_add_from_file(atlas,"../resources/fonts/DejaVuSansMono.ttf",14,&config);
    nk_glfw3_font_stash_end();
   //set the font
    if(font)
        nk_style_set_font(ctx,&font->handle);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
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
    /*
     = {
            0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,

            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,

            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,

            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,

            0.014f,  0.184f,  0.576f,
            0.771f,  0.328f,  0.970f,
            0.406f,  0.615f,  0.116f,

            0.676f,  0.977f,  0.133f,
            0.971f,  0.572f,  0.833f,
            0.140f,  0.616f,  0.489f,

            0.997f,  0.513f,  0.064f,
            0.945f,  0.719f,  0.592f,
            0.543f,  0.021f,  0.978f,

            0.279f,  0.317f,  0.505f,
            0.167f,  0.620f,  0.077f,
            0.347f,  0.857f,  0.137f,

            0.055f,  0.953f,  0.042f,
            0.714f,  0.505f,  0.345f,
            0.783f,  0.290f,  0.734f,

            0.722f,  0.645f,  0.174f,
            0.302f,  0.455f,  0.848f,
            0.225f,  0.587f,  0.040f,

            0.517f,  0.713f,  0.338f,
            0.053f,  0.959f,  0.120f,
            0.393f,  0.621f,  0.362f,

            0.673f,  0.211f,  0.457f,
            0.820f,  0.883f,  0.371f,
            0.982f,  0.099f,  0.879f
    };
      */

    /*random color*/
    srand(time(NULL));
    for (int i = 0; i < sizeof(g_color_buffer_data) / sizeof(GLfloat); ++i) {
        g_color_buffer_data[i] = GLfloat(rand() % 100) / 100.0f;
    }

    // we need a vertex array object (VAO) first.
    // a VAO can have mutiple vertex buffer object(VBO) belongs to it
    // just bind VAO,then bind VBO, past Data. All these VBO will relate with VAO
    // we do all bind once and then just use the current VAO object to draw directly,no need to bind any VBO again.
    GLuint vertexArrayId;
    glGenVertexArrays(1,&vertexArrayId);
    glBindVertexArray(vertexArrayId);
    //gen buffer
    GLuint vertexbuffer;            //identify for vertexBuffer
    glGenBuffers(1, &vertexbuffer); //gen 1 buffer,saved in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);    //bind the generated buffer, otherwise the following operation will take effect on it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
            0,//same as the bind location
            3,//size of the array element, just 3 float makes a point.
            GL_FLOAT,//we use float for point
            GL_FALSE,//not normalize
            0,//stride
            (void*)0//array buffer offset.
    );
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
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
    GLuint programId = Misc::loadShader("../resources/shaders/SimpleVertexShader.glsl","../resources/shaders/SimpleFragmentShader.glsl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //let's create the MVP matrix!
    glm::mat4 projectionMartrix = glm::perspective(glm::radians(45.0f),(float)width / (float)height,0.1f, 100.0f);
//    glm::mat4 projectionMartrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(4,3,3),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
    );
    glm::mat4 modelMatrix = glm::mat4( 1.0f);
    glm::mat4 mvpMatrix = projectionMartrix * viewMatrix * modelMatrix;
    GLint matrixId = glGetUniformLocation(programId, "MVP");

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        nk_glfw3_new_frame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programId);
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvpMatrix[0][0]);
        glfwGetWindowSize(window,&width,&height);
        glViewport(0,0,width,height);

        //ok we can do the openGL toturial demo test here.
        {
            glBindVertexArray(vertexArrayId);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glDrawArrays(GL_TRIANGLES,0, 12 * 3);

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        //call nk draw func.
#if 0
        drawMainGui(window);
#endif

        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1,&vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteVertexArrays(1,&vertexArrayId);
    glDeleteProgram(programId);

    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}
