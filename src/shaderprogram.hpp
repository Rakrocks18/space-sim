#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaders.hpp"

class ShaderProgram {
private:
    GLuint programId;
    
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource) {
        // Vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // Fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        // Shader program
        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);
        checkCompileErrors(programId, "PROGRAM");

        // Delete shaders as they're linked into the program and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~ShaderProgram() {
        glDeleteProgram(programId);
    }

    void use() const {
        glUseProgram(programId);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), static_cast<int>(value));
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(value));
    }

    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(value));
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    GLuint getId() const { return programId; }

private:
    void checkCompileErrors(GLuint shader, const std::string& type) {
        GLint success;
        char infoLog[1024];
        
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
