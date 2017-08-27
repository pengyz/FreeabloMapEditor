#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "fa_nuklear.h"
#include "nuklear_glfw_gl3.h"
#include "glShader.h"
#include "glm.hpp"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void drawMainGui(nk_context *ctx)
{
    //let's draw a simple ui.
    if(nk_begin(ctx, u8"Кнопка", nk_rect(50,50,400,400),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE)){
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
        std::cout << "glfwInit failed !" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
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

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };
    //we need a vertex array object (VAO) first.
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
    glBindVertexArray(0);
    GLuint programId = Misc::loadShader("../resources/shaders/SimpleVertexShader.glsl","../resources/shaders/SimpleFragmentShader.glsl");

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        nk_glfw3_new_frame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetWindowSize(window,&width,&height);
        glViewport(0,0,width,height);

        //call nk draw func.
#if 0
        drawMainGui(ctx);
#endif
        //ok we can do the openGL toturial demo test here.
        {
            glm::mat4 mat4(1);
            glUseProgram(programId);
            glBindVertexArray(vertexArrayId);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLES,0,3);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
            glUseProgram(0);
        }
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1,&vertexbuffer);
    glDeleteVertexArrays(1,&vertexArrayId);
    glDeleteProgram(programId);

    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}