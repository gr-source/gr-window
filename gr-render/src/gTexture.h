#pragma once

#include "common.h"

namespace grr {
    enum TextureFormat : u16 {
        TextureFormat_RGB = 1 << 1,
        TextureFormat_RGB332 = 1 << 2,
        TextureFormat_RGB444 = 1 << 3,
        TextureFormat_RGB565 = 1 << 4,
        TextureFormat_RGB888 = 1 << 5,

        TextureFormat_RGBA = 1 << 8,
        TextureFormat_RGBA4444 = 1 << 9,
        TextureFormat_RGBA8888 = 1 << 10
    };

    class gTexture {
    public:
        gTexture();
        
        static gTexture* Create(u32 width, u32 height, TextureFormat format, void* pixels);

        void bind();

        void unbind();

        const u32& getIndex() const;

        const bool isValid() const;

        void destroy();
    private:
        u32 m_height;
        u32 m_width;
        u32 m_index;

        TextureFormat m_format;

        static std::unordered_map<u32, u32> m_textureMap;
    };
} // namespace gr
