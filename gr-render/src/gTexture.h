#pragma once

#include "common.h"

namespace grr {
    enum TextureFlags : u8 {
        TextureFlags_RGB = 1 << 1,
        TextureFlags_RGBA = 1 << 2,
    };

    class gTexture {
    public:
        static gTexture* Create(u32 width, u32 height, u8 flags, const void* pixels);

        const u32& getIndex() const;

        void destroy();
    private:
        u32 m_height;
        u32 m_width;
        u32 m_index;
        u8 m_flags;
    };
} // namespace gr
