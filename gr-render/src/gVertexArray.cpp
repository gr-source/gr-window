#include "gVertexArray.h"

// #include "gRender.h"
#include "gl.h"

namespace grr {
    gVertexArray::gVertexArray() : m_bufferSize(0), m_vao(-1), m_vbo(-1), m_ebo(-1) {}

    gVertexArray* gVertexArray::m_instance = nullptr;
    u32 gVertexArray::m_bufferUsage = -1;

    std::unordered_map<u32, u32> gVertexArray::m_vertexArrayMap {
        {GR_VBO, GL_ARRAY_BUFFER},
        {GR_EBO, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<PrimitiveType, u32> gVertexArray::m_primitiveMap {
        {PrimitiveType_Points, GL_POINTS},
        {PrimitiveType_Lines, GL_LINES},
        {PrimitiveType_LineLoop, GL_LINE_LOOP},
        {PrimitiveType_LineStrip, GL_LINE_STRIP},
        {PrimitiveType_Triangles, GL_TRIANGLES},
        {PrimitiveType_TriangleStrip, GL_TRIANGLE_STRIP},
        {PrimitiveType_TriangleFan, GL_TRIANGLE_FAN}
    };

    gVertexArray *gVertexArray::Create(u32 bufferSize, u16 stride, bool hasElements) {
        gVertexArray* vertexArray = new gVertexArray();
        vertexArray->m_bufferSize = bufferSize;

        GL_CALL(glGenVertexArrays(1, &vertexArray->m_vao));
        GL_CALL(glBindVertexArray(vertexArray->m_vao));

        if (hasElements) {
            GL_CALL(glGenBuffers(1, &vertexArray->m_ebo));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->m_ebo));
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize * sizeof(u32), nullptr, GL_STATIC_DRAW));
        }

        GL_CALL(glGenBuffers(1, &vertexArray->m_vbo));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexArray->m_vbo));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, bufferSize * stride, nullptr, GL_STATIC_DRAW));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vertexArray;
    }

    void gVertexArray::Use(VertexArray vertex) {
        switch (vertex) {
        case GR_VBO:
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_instance->m_vbo));
            break;
        case GR_EBO:
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_instance->m_ebo));
            break;
        default:
            break;
        }
        m_bufferUsage = m_vertexArrayMap[vertex];
    }

    void gVertexArray::SetAttrib(u8 index, u16 size, u16 stride, const void *pointer) {
        GL_CALL(glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(size), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), pointer));
        GL_CALL(glEnableVertexAttribArray(static_cast<GLuint>(index)));
    }

    void gVertexArray::UpdateResize(u32 size, u16 stride) {
        int arraySize = 0;
        GL_CALL(glGetBufferParameteriv(m_bufferUsage, GL_BUFFER_SIZE,  &arraySize));
        arraySize = (arraySize / stride);
        
        while (size > arraySize) {
            GL_CALL(glBufferData(m_bufferUsage, ((arraySize * stride) + (GR_MAX_BLOCK_BUFFER * stride)), nullptr, GL_STATIC_DRAW));

            GL_CALL(glGetBufferParameteriv(m_bufferUsage, GL_BUFFER_SIZE,  &arraySize));
            arraySize = (arraySize / stride);
        }
    }

    void gVertexArray::SetBufferUpdate(u32 offset, u32 size, const void* data) {
        GL_CALL(glBufferSubData(m_bufferUsage, offset, size, data));
    }

    void gVertexArray::DrawElements(PrimitiveType primitive, u32 count, const void* indices) {
        GL_CALL(glDrawElements(m_primitiveMap[primitive], count, GL_UNSIGNED_INT, indices));
    }

    void gVertexArray::DrawArrays(PrimitiveType primitive, u32 count) {
        GL_CALL(glDrawArrays(m_primitiveMap[primitive], 0, count));
    }

    void gVertexArray::bind() {
        GL_CALL(glBindVertexArray(m_vao));

        m_instance = this;
    }

    void gVertexArray::unbind() {
        glBindVertexArray(0);

        m_instance = nullptr;
    }

    void gVertexArray::destroy() {
        if (m_vao != -1) {
            GL_CALL(glDeleteVertexArrays(1, &m_vao));
        }
        if (m_vbo != -1) {
            GL_CALL(glDeleteBuffers(1, &m_vbo));
        }
        if (m_ebo != -1) {
            GL_CALL(glDeleteBuffers(1, &m_ebo));
        }
        delete this;
    }
} // namespace grr



