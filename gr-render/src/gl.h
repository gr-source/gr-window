#pragma once

#include "gCommon.h"

#if GR_OPENGLES3
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif

namespace grr {
    const char* get_enum_name(GLenum err);

    void check_erros_opengl(const std::string& name);

    #define GL_CALL(func) func; check_erros_opengl(#func)
}