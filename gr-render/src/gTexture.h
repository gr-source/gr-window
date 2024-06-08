#pragma once

#include "gCommon.h"

namespace grr {
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
