#pragma once

#include "gCommon.h"

namespace grr {
    class gShader {
    public:
        gShader();

        static gShader* Create(const std::vector<const char*>& fragments, const std::vector<const char*>& vertex);

        static gShader* GetCurrent();

        template <typename T>
        static void SetUniform(const std::string& name, u16 count, const T* data);

        template <typename T>
        static void SetUniform(const std::string& name, const T data);

        void bind();

        void SetUniformMat4(const std::string& name, const Matrix4x4& value);

        void SetUniformInt(const std::string& name, int value);

        void unbind();

        void destroy();

    private:
        u32 m_index;

        static gShader* m_instance;

        static void checkerrors(u32 shader, bool compile);
    };
}