#pragma once

#include "gl.h"

namespace grr {
    inline std::string vertexShader2D = R"(
        layout (location = 0) in vec2 in_pos;
        layout (location = 1) in vec2 in_texCoord;

        out vec2 m_texCoord;

        uniform mat4 projection;

        void main() {
            m_texCoord = in_texCoord;

            gl_Position = projection * vec4(in_pos, 0.0, 1.0);
        }
    )";

    inline std::string fragmentShader2D = R"(
        out vec4 FragColor;

        in vec2 m_texCoord;

        uniform sampler2D u_texture;

        void main() {
            vec4 color = texture(u_texture, m_texCoord);
            if (color.a < 0.1) {
                discard;
            }
            
            FragColor = color;
        }
    )";

    inline std::string fragmentShader3D = R"(
        in vec3 m_fragPos;

        out vec4 FragColor;

        void main() {
            FragColor = vec4(m_fragPos, 1.0);
        }
    )";

    inline std::string vertexShader3D = R"(
        layout (location = 0) in vec3 in_pos;

        out vec3 m_fragPos;

        uniform mat4 projection;
        uniform mat4 view;

        void main() {
            gl_Position = projection * view * vec4(in_pos, 1.0);

            m_fragPos = in_pos;
        }
    )";

    inline std::string vertexShaderInstance3D = R"(
        layout (location = 0) in vec3 in_pos;
        layout (location = 1) in mat4 in_model;

        out vec3 m_fragPos;

        uniform mat4 projection;
        uniform mat4 view;

        void main() {
            gl_Position = projection * view * in_model * vec4(in_pos, 1.0);

            m_fragPos = in_pos;
        }
    )";



} // namespace grr
