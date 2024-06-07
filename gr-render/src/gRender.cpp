#include "gRender.h"

#include "gTexture.h"
#include "gShader.h"
#include "gl.h" 

#include "gColor.h"

namespace grr {
    const char* vertexShader2D = R"(
        #version 330 core

        layout (location = 0) in vec2 in_pos;
        layout (location = 1) in vec2 in_texCoord;

        out vec2 m_texCoord;

        uniform mat4 projection;

        void main() {
            m_texCoord = in_texCoord;

            gl_Position = projection * vec4(in_pos, 0.0, 1.0);
        }
    )";

    const char* fragmentShader2D = R"(
        #version 330 core

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

    const char* fragmentShader3D = R"(
        #version 330 core

        in vec3 m_fragPos;

        out vec4 FragColor;

        void main() {
            FragColor = vec4(m_fragPos, 1.0);
        }
    )";

    const char* vertexShader3D = R"(
        #version 330 core

        layout (location = 0) in vec3 in_pos;

        out vec3 m_fragPos;

        uniform mat4 projection;
        uniform mat4 view;

        void main() {
            gl_Position = projection * view * vec4(in_pos, 1.0);

            m_fragPos = in_pos;
        }
    )";

    std::unordered_map<PrimitiveType, u32> gRender::m_primitiveMap {
        {PrimitiveType_Points, GL_POINTS},
        {PrimitiveType_Lines, GL_LINES},
        {PrimitiveType_LineLoop, GL_LINE_LOOP},
        {PrimitiveType_LineStrip, GL_LINE_STRIP},
        {PrimitiveType_Triangles, GL_TRIANGLES},
        {PrimitiveType_TriangleStrip, GL_TRIANGLE_STRIP},
        {PrimitiveType_TriangleFan, GL_TRIANGLE_FAN}
    };

    std::unordered_map<BufferBindingTarget, u32> gRender::m_bufferMap {
        {BufferBindingTarget::GR_ARRAY_BUFFER, GL_ARRAY_BUFFER},
        {BufferBindingTarget::GR_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER}
    };

    gShader* gRender::m_shader2D = nullptr;
    gShader* gRender::m_shader3D = nullptr;

    u32 gRender::m_height = 0;
    u32 gRender::m_width = 0;

    void gRender::SetRenderState(RenderState state, void *value) {
        switch (state) {
        case GR_BACKGROUND_COLOR: {
            gColor* color = (gColor*)value;
            GL_CALL(glClearColor(color->r, color->g, color->b, color->a));
            break;
        }
        case GR_VIEWPORT: {
            Vector2* size = (Vector2*)value;
            GL_CALL(glViewport(0, 0, size->x, size->y));
            break;
        }
        default:
            std::cout << "Invalid V: " << getRenderStateName(state) << std::endl;
            break;
        }
    }

    void gRender::SetRenderState(RenderState state, u16 value) {
        switch (state) {
        case GR_BACKGROUND: {
            GLbitfield filter = 0;
            if ((value & GR_DEPTH_BUFFER) == GR_DEPTH_BUFFER) {
                filter |= GL_DEPTH_BUFFER_BIT;
            }
            if ((value & GR_COLOR_BUFFER) == GR_COLOR_BUFFER) {
                filter |= GL_COLOR_BUFFER_BIT;
            }
            return GL_CALL(glClear(filter));
        }
        case GR_CULL_FACE: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_CULL_FACE));
            }
            return GL_CALL(glDisable(GL_CULL_FACE));
        }
        case GR_CULL: {
            if ((value & GR_FRONT) == GR_FRONT && (value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_FRONT_AND_BACK));
            } else if ((value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_BACK));
            }
            return GL_CALL(glCullFace(GL_FRONT));
        }
        case GR_DEPTH: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_DEPTH_TEST));
            }
            return GL_CALL(glDisable(GL_DEPTH_TEST));
        }
        default:
            std::cout << "Invalid Va: " << getRenderStateName(state) << std::endl;
        }
    }

    const std::string gRender::getRenderStateName(RenderState state) {
        #define GET_ENUM_NAME(e) case e: return #e
        switch (state) {
        GET_ENUM_NAME(GR_FALSE);
        GET_ENUM_NAME(GR_TRUE);
        GET_ENUM_NAME(GR_BACKGROUND_COLOR);
        GET_ENUM_NAME(GR_BACKGROUND);
        GET_ENUM_NAME(GR_DEPTH_BUFFER);
        GET_ENUM_NAME(GR_COLOR_BUFFER);
        GET_ENUM_NAME(GR_CULL_FACE);
        GET_ENUM_NAME(GR_CULL);
        GET_ENUM_NAME(GR_FRONT);
        GET_ENUM_NAME(GR_BACK);
        GET_ENUM_NAME(GR_DEPTH);
        GET_ENUM_NAME(GR_VIEWPORT);
        default:
            return "undefined";
        }
        #undef GET_ENUM_NAME
    }

    u32 vao2D, vbo2D, ebo2D;
    u32 vao3D, vbo3D, ebo3D;

    const bool gRender::OpenGLInit() {
        #if !GR_OPENGLES3
        if (glewInit() != GLEW_OK) {
            return false;
        }
        #endif

        CreateVertexArray2D();
        m_shader2D = gShader::Create({fragmentShader2D}, {vertexShader2D});

        CreateVertexArray3D();
        m_shader3D = gShader::Create({fragmentShader3D}, {vertexShader3D});

        return true;
    }

    void gRender::SetResolution(u32 width, u32 height) {
        m_height = height;
        m_width = width;
    }

    void gRender::Render2D() {
        Matrix4x4 projection = Math::orthographic(0.0f, m_width, 0.0f, m_height, -1.0f, 1.0f);
        
        m_shader2D->bind();
        m_shader2D->SetUniformMat4("projection", projection);
    }

    void gRender::RenderTexture2D(const Matrix3x3 &model, gTexture *texture) {
    }

    void gRender::RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice) {
        GL_CALL(glBindVertexArray(vao2D));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo2D));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, numVertice * sizeof(Vertex2D), vertice));
        
        GL_CALL(glDrawArrays(m_primitiveMap[primitive], 0, numVertice));
    }

    void gRender::RenderIndexedPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        GL_CALL(glBindVertexArray(vao2D));

        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2D));
        GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndice * sizeof(u32), indice));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo2D));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, numVertice * sizeof(Vertex2D), vertice));

        GL_CALL(glDrawElements(m_primitiveMap[primitive], numIndice, GL_UNSIGNED_INT, nullptr));
    }

    void gRender::Render3D(const Matrix4x4& projection, const Matrix4x4& view) {
        m_shader3D->bind();
        m_shader3D->SetUniformMat4("projection", projection);
        m_shader3D->SetUniformMat4("view", view);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice) {
        GL_CALL(glBindVertexArray(vao3D));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo3D));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, numVertice * sizeof(Vertex3D), vertice));

        GL_CALL(glDrawArrays(m_primitiveMap[primitive], 0, numVertice));
    }

    void gRender::RenderIndexedPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        // Use o vão 
        GL_CALL(glBindVertexArray(vao3D));

        // element buffer
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo3D));
        ReizeBuffer(GR_ELEMENT_ARRAY_BUFFER, sizeof(u32), numIndice);
        GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndice * sizeof(u32), indice));

        // array buffer
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo3D));
        ReizeBuffer(GR_ARRAY_BUFFER, sizeof(Vertex3D), numVertice);
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, numVertice * sizeof(Vertex3D), vertice));

        // draw
        GL_CALL(glDrawElements(m_primitiveMap[primitive], numIndice, GL_UNSIGNED_INT, nullptr));
    }

    void gRender::OpenGLShutsown() {
        if (m_shader2D) {
            m_shader2D->destroy();
        }
        if (m_shader3D) {
            m_shader3D->destroy();
        }
    }

    void gRender::CreateVertexArray2D() {
        GL_CALL(glGenVertexArrays(1, &vao2D));
        GL_CALL(glBindVertexArray(vao2D));

        GL_CALL(glGenBuffers(1, &ebo2D));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2D));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_BLOCK_BUFFER * sizeof(u32), nullptr, GL_STATIC_DRAW));

        GL_CALL(glGenBuffers(1, &vbo2D));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo2D));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, MAX_BLOCK_BUFFER * sizeof(Vertex2D), nullptr, GL_STATIC_DRAW));

        GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void*>(offsetof(Vertex2D, position))));
        GL_CALL(glEnableVertexAttribArray(0));

        GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void*>(offsetof(Vertex2D, uv))));
        GL_CALL(glEnableVertexAttribArray(1));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void gRender::CreateVertexArray3D() {
        GL_CALL(glGenVertexArrays(1, &vao3D));
        GL_CALL(glBindVertexArray(vao3D));

        GL_CALL(glGenBuffers(1, &ebo3D));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo3D));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_BLOCK_BUFFER * sizeof(u32), nullptr, GL_STATIC_DRAW));

        GL_CALL(glGenBuffers(1, &vbo3D));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo3D));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, MAX_BLOCK_BUFFER * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW));

        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(offsetof(Vertex3D, position))));
        GL_CALL(glEnableVertexAttribArray(0));

        GL_CALL(glBindVertexArray(0));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void gRender::ReizeBuffer(BufferBindingTarget target, u16 stride, u32 size) {
        int arraySize = 0;
        GL_CALL(glGetBufferParameteriv(m_bufferMap[target], GL_BUFFER_SIZE,  &arraySize));
        arraySize = (arraySize / stride);
        
        while (size > arraySize) {
            GL_CALL(glBufferData(m_bufferMap[target], arraySize * stride + MAX_BLOCK_BUFFER * stride, nullptr, GL_STATIC_DRAW));

            GL_CALL(glGetBufferParameteriv(m_bufferMap[target], GL_BUFFER_SIZE,  &arraySize));
            arraySize = (arraySize / stride);
        }
    }
}
