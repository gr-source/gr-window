#include "gShader.h"

#include "gl.h"

namespace grr {
    gShader* gShader::m_instance = nullptr;

    gShader::gShader() : m_index(0) {}

    gShader *gShader::Create(const std::vector<const char*>& fragments, const std::vector<const char*>& vertex) {
        gShader* shader = new gShader();
        shader->m_index = GL_CALL(glCreateProgram());
        
        u32 fragmentShaderID = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
        GL_CALL(glShaderSource(fragmentShaderID, fragments.size(), fragments.data(), nullptr));
        GL_CALL(glCompileShader(fragmentShaderID));
        gShader::checkerrors(fragmentShaderID, true);

        u32 vertexShaderID = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
        GL_CALL(glShaderSource(vertexShaderID, vertex.size(), vertex.data(), nullptr));
        GL_CALL(glCompileShader(vertexShaderID));
        gShader::checkerrors(vertexShaderID, true);

        GL_CALL(glAttachShader(shader->m_index, fragmentShaderID));
        GL_CALL(glAttachShader(shader->m_index, vertexShaderID));
        GL_CALL(glLinkProgram(shader->m_index));
        gShader::checkerrors(shader->m_index, false);

        GL_CALL(glDeleteShader(fragmentShaderID));
        GL_CALL(glDeleteShader(vertexShaderID));

        return shader;
    }

    gShader *gShader::GetCurrent() {
        return gShader::m_instance;
    }

    template <>
    void gShader::SetUniform(const std::string& name, const int* data) {
        GLint location = GL_CALL(glGetUniformLocation(m_instance->m_index, name.c_str()));
        if (location != -1) {
            GL_CALL(glUniform1i(location, *static_cast<const GLint*>(data)));
        }
    }

    void gShader::bind() {
        GL_CALL(glUseProgram(m_index));
        gShader::m_instance = this;
    }

    void gShader::SetUniformMat4(const std::string &name, const Matrix4x4 &value) {
        GLint location = GL_CALL(glGetUniformLocation(m_index, name.c_str()));
        if (location != -1) {
            GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, value.data));
        }
    }

    void gShader::SetUniformInt(const std::string &name, int value) {
        GLint location = GL_CALL(glGetUniformLocation(m_index, name.c_str()));
        if (location != -1) {
            GL_CALL(glUniform1i(location, value));
        }
    }

    void gShader::unbind() {
        GL_CALL(glUseProgram(0));
        gShader::m_instance = nullptr;
    }

    void gShader::destroy() {
        delete this;
    }

    void gShader::checkerrors(u32 shader, bool compile) {
        GLint status;
        if (compile) {
            GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
            if (status != GL_TRUE) {
                GLint length;
                GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

                GLint type;
                GL_CALL(glGetShaderiv(shader, GL_SHADER_TYPE, &type));

                std::string logMessage(length, '\0');
                GL_CALL(glGetShaderInfoLog(shader, logMessage.size(), nullptr, logMessage.data()));

                std::cout << "Failed to compile shader - ";
                switch (type) {
                case GL_FRAGMENT_SHADER:
                    std::cout << ".frag" << std::endl << std::endl;
                    break;
                case GL_VERTEX_SHADER:
                    std::cout << ".vert" << std::endl << std::endl;
                    break;
                default:
                    break;
                }
                std::cout << logMessage << std::endl;
            }
            return;
        }

        GL_CALL(glGetProgramiv(shader, GL_LINK_STATUS, &status));
        if (status != GL_TRUE) {
            std::string logMessage(512, '\0');

            GL_CALL(glGetProgramInfoLog(shader, logMessage.size(), nullptr, logMessage.data()));
            std::cout << "Failed to link shader program - " << std::endl << std::endl;
            std::cout << logMessage << std::endl;
        }
    }
} // namespace grr
