#include "gTexture.h"

#include "gl.h"

namespace grr {
    gTexture *gTexture::Create(u32 width, u32 height, u8 flags, const void *pixels) {
        gTexture* texture = new gTexture();
        texture->m_width = width;
        texture->m_height = height;
        texture->m_flags = flags;

        GL_CALL(glGenTextures(1, &texture->m_index));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->m_index));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        return texture;
    }

    const u32& gTexture::getIndex() const {
        return m_index;
    }

    void gTexture::destroy() {
        delete this;
    }
} // namespace gr

