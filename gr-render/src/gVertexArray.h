#pragma once

#include "gCommon.h"

#define GR_MAX_BLOCK_BUFFER 1024

namespace grr {
    class gVertexArray {
    public:
        gVertexArray();
        
        static gVertexArray* Create(u32 bufferSize, u16 stride, bool hasElements);

        static u32 CreateBuffer(BufferType target, u32 size, u16 stride);

        static void Bind(u32 index);

        static void SetAttrib(u8 index, u16 size, u16 stride, const void* pointer);

        static void UpdateResize(u32 index, u32 size, u16 stride);

        static void SetBufferUpdate(u32 index, u32 offset, u32 size, const void* data);

        static void DrawElementsInstanced(PrimitiveType primitive, u32 count, const void* indices, u32 primcount);

        static void DrawElements(PrimitiveType primitive, u32 count, const void* indices);

        static void DrawArrays(PrimitiveType primitive, u32 count);

        void bind();

        void unbind();

        void destroy();
    private:
        u32 m_bufferSize;
        u32 m_vao;

        static gVertexArray* m_instance;

        static std::unordered_map<BufferType, u32> m_bufferTypeMap;

        static std::unordered_map<PrimitiveType, u32> m_primitiveMap;

        static std::unordered_map<u32, u32> m_bufferIndex;
    };
} // namespace grr
