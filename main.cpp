#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "fa_nuklear.h"
#include "nuklear_glfw_gl3.h"


#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main() {
    //ok,let's init glfw and create a window
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


    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        nk_glfw3_new_frame();

        //let's draw a simple ui.
        {
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
        }

        glfwGetWindowSize(window,&width,&height);
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);


        nk_glfw3_render(NK_ANTI_ALIASING_ON,MAX_VERTEX_BUFFER,MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(window);
    }

    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}