#pragma once

#include "common.h"

#include "gColor.h"

namespace grr {
    enum RenderState : u8 {
        GR_FALSE            = 0x000,
        GR_TRUE             = 0x001,
        GR_BACKGROUND_COLOR = 1 << 1,
        GR_BACKGROUND       = 1 << 2,
        GR_DEPTH_BUFFER     = 1 << 3,
        GR_COLOR_BUFFER     = 1 << 4,
        GR_CULL_FACE        = 1 << 5,
        GR_CULL             = 1 << 6,
        GR_FRONT            = 1 << 7,
        GR_BACK             = 2 << 1,
        GR_DEPTH            = 2 << 2
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

        static void SetRenderState(RenderState state, u8 value);

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