#include "gRender.h"

#include "gShaderSource.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gl.h" 

#include "gColor.h"

namespace grr {
    std::unordered_map<BufferBindingTarget, u32> gRender::m_bufferMap {
        {BufferBindingTarget::GR_ARRAY_BUFFER, GL_ARRAY_BUFFER},
        {BufferBindingTarget::GR_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER}
    };

    gVertexArray* gRender::m_vertexArray2D = nullptr;
    gVertexArray* gRender::m_vertexArray3D = nullptr;

    gShader* gRender::m_shader2D = nullptr;
    gShader* gRender::m_shader3D = nullptr;
    gShader* gRender::m_shaderInstanced3D = nullptr;

    u32 gRender::m_height = 0;
    u32 gRender::m_width = 0;

    u32 indexPrimi3D = 0;
    u32 indexVbo3D = 0;
    u32 indexEbo3D = 0;

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

    const bool gRender::OpenGLInit() {
        #if !GR_OPENGLES3
        if (glewInit() != GLEW_OK) {
            return false;
        }
        #endif

        #if GR_OPENGLES3
        const std::string glVersion = "#version 300 es";
        #else
        const std::string glVersion = "#version 330 core";
        #endif

        CreateVertexArray2D();
        m_shader2D = gShader::Create({(glVersion + fragmentShader2D).c_str()}, {(glVersion + vertexShader2D).c_str()});

        CreateVertexArray3D();
        m_shader3D = gShader::Create({(glVersion + fragmentShader3D).c_str()}, {(glVersion + vertexShader3D).c_str()});

        m_shaderInstanced3D = gShader::Create({(glVersion + fragmentShader3D).c_str()}, {(glVersion + vertexShaderInstance3D).c_str()});

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
        // m_vertexArray2D->bind();

        // gVertexArray::Use(BufferType_VBO);
        // gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex2D), vertice);

        // gVertexArray::DrawArrays(primitive, numVertice);
    }

    void gRender::RenderIndexedPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        // m_vertexArray2D->bind();

        // gVertexArray::Use(BufferType_EBO);
        // gVertexArray::SetBufferUpdate(0, numIndice * sizeof(u32), indice);

        // gVertexArray::Use(BufferType_VBO);
        // gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex2D), vertice);

        // gVertexArray::DrawElements(primitive, numIndice, nullptr);
    }

    void gRender::Render3D(const Matrix4x4& projection, const Matrix4x4& view) {
        m_shaderInstanced3D->bind();
        m_shaderInstanced3D->SetUniformMat4("projection", projection);
        m_shaderInstanced3D->SetUniformMat4("view", view);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(indexVbo3D);
        gVertexArray::UpdateResize(indexVbo3D, numVertice, sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(indexVbo3D, 0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::DrawArrays(primitive, numVertice);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(indexEbo3D);
        gVertexArray::UpdateResize(indexEbo3D, numIndice, sizeof(u32));
        gVertexArray::SetBufferUpdate(indexEbo3D, 0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(indexVbo3D);
        gVertexArray::UpdateResize(indexVbo3D, numVertice, sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(indexVbo3D, 0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::DrawElements(primitive, numIndice, nullptr);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice, u32 numPrim, void *prim) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(indexEbo3D);
        gVertexArray::UpdateResize(indexEbo3D, numIndice, sizeof(u32));
        gVertexArray::SetBufferUpdate(indexEbo3D, 0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(indexVbo3D);
        gVertexArray::UpdateResize(indexVbo3D, numVertice, sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(indexVbo3D, 0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::Bind(indexPrimi3D);
        gVertexArray::UpdateResize(indexPrimi3D, numPrim, sizeof(Matrix4x4));
        gVertexArray::SetBufferUpdate(indexPrimi3D, 0, numPrim * sizeof(Matrix4x4), prim);

        gVertexArray::DrawElementsInstanced(PrimitiveType_Triangles, numIndice, nullptr, numPrim);
    }

    void gRender::CreateVertexArray2D() {
        // m_vertexArray2D = gVertexArray::Create(MAX_BLOCK_BUFFER, sizeof(gVertex2D), true);

        // m_vertexArray2D->bind();

        // gVertexArray::Use(BufferType_VBO);

        // gVertexArray::SetAttrib(0, 2, sizeof(gVertex2D), reinterpret_cast<void*>(offsetof(gVertex2D, position)));
        // gVertexArray::SetAttrib(1, 2, sizeof(gVertex2D), reinterpret_cast<void*>(offsetof(gVertex2D, uv)));

        // m_vertexArray2D->unbind();
    }

    void gRender::CreateVertexArray3D() {
        m_vertexArray3D = gVertexArray::Create(MAX_BLOCK_BUFFER, sizeof(gVertex3D), true);
        m_vertexArray3D->bind();

        indexEbo3D = gVertexArray::CreateBuffer(BufferType_EBO, MAX_BLOCK_BUFFER, sizeof(u32));

        indexVbo3D = gVertexArray::CreateBuffer(BufferType_VBO, MAX_BLOCK_BUFFER, sizeof(gVertex3D));
        gVertexArray::Bind(indexVbo3D);
        gVertexArray::SetAttrib(0, 3, sizeof(gVertex3D), reinterpret_cast<void*>(offsetof(gVertex3D, position)));

        indexPrimi3D = gVertexArray::CreateBuffer(BufferType_DEFAULT, MAX_BLOCK_BUFFER, sizeof(Matrix4x4));
        gVertexArray::Bind(indexPrimi3D);

        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (GLvoid*)(sizeof(Vector4) * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        m_vertexArray3D->unbind();
    }

    void gRender::OpenGLShutsown() {
        if (m_shader2D) {
            m_shader2D->destroy();
        }
        if (m_shader3D) {
            m_shader3D->destroy();
        }

        if (m_vertexArray2D) {
            m_vertexArray2D->destroy();
        }
        if (m_vertexArray3D) {
            m_vertexArray3D->destroy();
        }
    }
}
