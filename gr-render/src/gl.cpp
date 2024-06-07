#include "gl.h"

namespace grr {
    const char* get_enum_name(GLenum err) {
        #define GETENUMNAME(e) case e: return #e;
        switch (err) {
            /* ERROR */
            GETENUMNAME(GL_INVALID_ENUM);
            GETENUMNAME(GL_INVALID_VALUE);
            GETENUMNAME(GL_INVALID_OPERATION);
            GETENUMNAME(GL_INVALID_FRAMEBUFFER_OPERATION);
            GETENUMNAME(GL_OUT_OF_MEMORY);
            #if !GR_OPENGLES3
            GETENUMNAME(GL_STACK_UNDERFLOW);
            GETENUMNAME(GL_STACK_OVERFLOW);
            GETENUMNAME(GL_CONTEXT_LOST);
            #endif
        default:
            return "Unknown";
        }
        #undef GETENUMNAME
    }

    void check_erros_opengl(const std::string &name) {
        u32 numErrors = 0;
        GLenum err;

        while ((err = glGetError()) != GL_NO_ERROR) {
            numErrors++;
            std::cout << "Opengl error: " << name << "(" << get_enum_name(err) << ")" << std::endl;
        }
    }
} // namespace grr
