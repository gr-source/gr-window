#pragma once

#include "common.h"
#include "gColor.h"

#include <sstream>
#include <iomanip>
#include <string>

#define MAX_BLOCK_BUFFER 1024
#define GR_OPENGLES3 0

namespace grr {
    enum RenderState : u16 {
        GR_FALSE            = 0x000,
        GR_TRUE             = 0x001,
        GR_BACKGROUND_COLOR = 0x002,
        GR_BACKGROUND       = 0x004,
        GR_DEPTH_BUFFER     = 0x008,
        GR_COLOR_BUFFER     = 0x010,
        GR_CULL_FACE        = 0x020,
        GR_CULL             = 0x040,
        GR_FRONT            = 0x080,
        GR_BACK             = 0x100,
        GR_DEPTH            = 0x200,
        GR_VIEWPORT         = 0x400
    };

    enum PrimitiveType {
        PrimitiveType_Points = 0,
        PrimitiveType_Lines = 1,
        PrimitiveType_LineLoop = 2,
        PrimitiveType_LineStrip = 3,
        PrimitiveType_Triangles = 4,
        PrimitiveType_TriangleStrip = 5,
        PrimitiveType_TriangleFan = 6
    };

    enum BufferBindingTarget {
        GR_ARRAY_BUFFER              = 1 << 1,
        // GR_COPY_READ_BUFFER          = 1 << 3,
        // GR_COPY_WRITE_BUFFER         = 1 << 4,
        // GR_DRAW_INDIRECT_BUFFER      = 1 << 5,
        GR_ELEMENT_ARRAY_BUFFER      = 1 << 6,
        // GR_PIXEL_PACK_BUFFER         = 1 << 7,
        // GR_PIXEL_UNPACK_BUFFER       = 1 << 8,
        // GR_TEXTURE_BUFFER            = 1 << 11, // OpenGL ES 3.1
        // GR_TRANSFORM_FEEDBACK_BUFFER = 1 << 12,
        // GR_UNIFORM_BUFFER            = 1 << 13,
    };

    struct Vertex2D {
        Vector2 position;
        Vector2 uv;
    };

    struct Vertex3D {
        Vector3 position;
    };

    class gRender {
    public:

        static void SetRenderState(RenderState state, void* value);

        static void SetRenderState(RenderState state, u16 value);

        static const std::string getRenderStateName(RenderState state);

        static const bool OpenGLInit();

        static void SetResolution(u32 width, u32 height);

        static void Render2D();

        static void RenderTexture2D(const Matrix3x3& model, gTexture* texture);

        static void RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void* vertice);

        static void RenderIndexedPrimitive2D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice);

        static void Render3D(const Matrix4x4& projection, const Matrix4x4& model);

        static void RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void* vertice);

        static void RenderIndexedPrimitive3D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice);

        static void OpenGLShutsown();
    private:
        static std::unordered_map<BufferBindingTarget, u32> m_bufferMap;
        static std::unordered_map<PrimitiveType, u32> m_primitiveMap;

        static gShader* m_shader2D;
        static gShader* m_shader3D;

        static u32 m_height;
        static u32 m_width;

        static void CreateVertexArray2D();

        static void CreateVertexArray3D();

        static void ReizeBuffer(BufferBindingTarget target, u16 stride, u32 size);
    };
}