#pragma once

#include "common.h"

#include <GL/glew.h>

namespace grr {
    const char* get_enum_name(GLenum err);

    void check_erros_opengl(const std::string& name);

    #define GL_CALL(func) func; check_erros_opengl(#func)
}