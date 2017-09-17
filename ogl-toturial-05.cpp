#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glShader.h"
#include <iostream>
#include <string>
#include "glShader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main( void )
{
    //ok,let's init glfw and create a window
    glfwSetErrorCallback([](int errorCode,const char* errorMsg){
        std::cerr << "[" << errorCode << "] " << errorMsg <<  std::endl;
    });

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow( 1024, 768, "Tutorial 0 - Keyboard and Mouse", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    GLenum ret = glewInit();

    if ( ret!= GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW %d %s\n",ret, glewGetErrorString(ret));
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen

    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = Misc::loadShader( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Load the texture
//    GLuint Texture = loadDDS("uvtemplate.DDS");
    GLuint Texture = 0;

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
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

    // Two UV coordinatesfor each vertex. They were created with Blender.
    static const GLfloat g_uv_buffer_data[] = {
            0.000059f, 0.000004f,
            0.000103f, 0.336048f,
            0.335973f, 0.335903f,
            1.000023f, 0.000013f,
            0.667979f, 0.335851f,
            0.999958f, 0.336064f,
            0.667979f, 0.335851f,
            0.336024f, 0.671877f,
            0.667969f, 0.671889f,
            1.000023f, 0.000013f,
            0.668104f, 0.000013f,
            0.667979f, 0.335851f,
            0.000059f, 0.000004f,
            0.335973f, 0.335903f,
            0.336098f, 0.000071f,
            0.667979f, 0.335851f,
            0.335973f, 0.335903f,
            0.336024f, 0.671877f,
            1.000004f, 0.671847f,
            0.999958f, 0.336064f,
            0.667979f, 0.335851f,
            0.668104f, 0.000013f,
            0.335973f, 0.335903f,
            0.667979f, 0.335851f,
            0.335973f, 0.335903f,
            0.668104f, 0.000013f,
            0.336098f, 0.000071f,
            0.000103f, 0.336048f,
            0.000004f, 0.671870f,
            0.336024f, 0.671877f,
            0.000103f, 0.336048f,
            0.336024f, 0.671877f,
            0.335973f, 0.335903f,
            0.667969f, 0.671889f,
            1.000004f, 0.671847f,
            0.667979f, 0.335851f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    do{

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
//        computeMatricesFromInputs();
//        glm::mat4 ProjectionMatrix = getProjectionMatrix();
//        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ProjectionMatrix = glm::mat4(1);
        glm::mat4 ViewMatrix = glm::mat4(1);
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size : U+V => 2
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}









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

int main2(int argc, const char** argv)
{
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window_info *info = new window_info;
    info->width = 800;
    info->height = 600;
    info->title = "ogl-toturial-texture-clube";
    GLFWwindow *window = glfwCreateWindow(info->width,info->height,info->title.c_str(),nullptr, nullptr);
    if(!window){
        delete info;
        std::cout << " use glfwCreateWindow failed !" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(window,info);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err){
        std::cerr << " use glew versin:" << glewGetString(GLEW_VERSION) << ",call glewInit error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    const GLubyte* byteGlVersion = glGetString(GL_VERSION);
    const GLubyte* byteGlVendor = glGetString(GL_VENDOR);
    const GLubyte* byteGlRenderer = glGetString(GL_RENDERER);
    const GLubyte* byteSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "glVersion: " << byteGlVendor << std::endl;
    std::cout << "vender:" << byteGlVendor << std::endl;
    std::cout << "glRenderer:" << byteGlRenderer << std::endl;
    std::cout << "glslVersion:" << byteSLVersion << std::endl;

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