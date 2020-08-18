// https://learnopengl.com/Getting-started/Shaders
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath);

    // activate the shader
    // ------------------------------------------------------------------------
    // Modified by me (Antoine)
    void use() {
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);

        if(id != ID)
            glUseProgram(ID);
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    // --- Extensions
    void setVec3(const std::string &name, glm::vec3 value) const {
        glUniform3f(
                glGetUniformLocation(ID, name.c_str()),
                value[0],
                value[1],
                value[2]
        );
    }

    void setVec4(const std::string &name, glm::vec4 value) const {
        glUniform4f(
                glGetUniformLocation(ID, name.c_str()),
                value[0],
                value[1],
                value[2],
                value[3]
        );
    }

    void setMat4(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(
                glGetUniformLocation(ID, name.c_str()),
                1,
                GL_FALSE,
                &value[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif