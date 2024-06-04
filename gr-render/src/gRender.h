#pragma once

#include "common.h"

#include "gColor.h"

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
        static std::unordered_map<PrimitiveType, u32> m_primitiveMap;

        static gShader* m_shader2D;
        static gShader* m_shader3D;

        static u32 m_height;
        static u32 m_width;
    };
}