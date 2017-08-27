//
// Created by peng on 17-8-27.
//

#ifndef FREEABLOMAPEDITOR_LOADSHADER_H
#define FREEABLOMAPEDITOR_LOADSHADER_H

#include <GL/glew.h>
#include <string>

namespace Misc{
    std::string readShaderSource(const char* shader_file_path);
    bool compileShader(const std::string& shaderSource, std::string& errorLog);
    GLuint loadShader(const char* vertex_file_path, const char* fragment_file_path);
}


#endif //FREEABLOMAPEDITOR_LOADSHADER_H
