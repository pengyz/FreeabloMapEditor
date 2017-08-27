//
// Created by peng on 17-8-27.
//

#include "glShader.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

namespace Misc{

    std::string readShaderSource(const char* shader_file_path){
        std::string retString = "";
        std::ifstream vertexShaderStream(shader_file_path, std::ios::in);
        if(vertexShaderStream.is_open()){
            std::string line = "";
            while(getline(vertexShaderStream,line))
                retString += "\n" + line;
            vertexShaderStream.close();
        } else{
            getchar();
            return "";
        }
        return retString;
    }

    bool compileShader(GLuint shaderId, const std::string& shaderSource, std::string& errorLog)
    {
        assert(!shaderSource.empty());

        GLint result = GL_FALSE;
        int infoLogLen = 0;

        const char* shaderSourcePointer = shaderSource.c_str();
        glShaderSource(shaderId, 1, &shaderSourcePointer, NULL);
        glCompileShader(shaderId);
        //validate it
        glGetShaderiv(shaderId ,GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
        if(infoLogLen > 0){
            std::vector<char> infoLogBuf(infoLogLen + 1);
            glGetShaderInfoLog(shaderId, infoLogLen, NULL, &infoLogBuf[0]);
            errorLog = &infoLogBuf[0];
            return false;
        }
        return true;
    }

    GLuint loadShader(const char *vertex_file_path, const char *fragment_file_path) {
        GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        std::string vertexShaderCode = readShaderSource(vertex_file_path);
        if(vertexShaderCode.empty()) {
            std::cerr << "Impossible to open " << vertex_file_path << ". Make sure you have the right directory !" << std::endl;
            return 0;
        }
        std::string fragmentShaderSource = readShaderSource(fragment_file_path);
        if(fragmentShaderSource.empty()){
            std::cerr << "Impossible to open " << fragment_file_path << ". Make sure you have the right directory !" << std::endl;
            return 0;
        }
        std::string errorInfo;
        if(!compileShader(vertexShaderId, vertexShaderCode, errorInfo)){
            std::cerr << "compile vertex shader fialed ! with log: " << errorInfo.c_str() << std::endl;
            return 0;
        }
        if(!compileShader(fragmentShaderId, fragmentShaderSource, errorInfo)){
            std::cerr << "compile vertex shader fialed ! with log: " << errorInfo.c_str() << std::endl;
            return 0;
        }
        //link program
        GLuint programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        glLinkProgram(programId);
        GLint linkResult = GL_FALSE;
        int infoLen = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 0){
            std::vector<char> programErrorMsg(infoLen + 1);
            glGetProgramInfoLog(programId,infoLen,NULL,&programErrorMsg[0]);
            std::cerr << "link program error: " << &programErrorMsg[0];
        }

        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return programId;
    }
}
