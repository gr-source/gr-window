#pragma once

#include "common.h"

namespace grr {
    class gShader {
    public:
        gShader();

        static gShader* Create(const std::vector<const char*>& fragments, const std::vector<const char*>& vertex);

        void bind();

        void SetUniformMat4(const std::string& name, const Matrix4x4& value);

        void SetUniformInt(const std::string& name, int value);

        void unbind();

        void destroy();

    private:
        u32 m_index;

        static gShader* instance;

        static void checkerrors(u32 shader, bool compile);
    };
}