project("gr-render")

option(GR_COMPILE_LINUX "Compile for Linux" ON)

set(render_src
    src/gVertexArray.cpp
    src/gTexture.cpp
    src/gRender.cpp
    src/gShader.cpp
    src/gl.cpp
)

set(render_hdr
    src/gVertexArray.h
    src/gVertex2D.h
    src/gVertex3D.h
    src/gTexture.h
    src/gRender.h
    src/gShader.h
    src/gCommon.h
    src/gColor.h
    src/gTypes.h
)

add_library(grender STATIC ${render_src} ${render_hdr})

find_package(gr-math REQUIRED)

target_link_libraries(grender PRIVATE ${GRMATH_LIBRARIES})

target_compile_definitions(grender PUBLIC GR_OPENGLES3=0)

if(GR_COMPILE_LINUX)
    option(GR_USE_GLEW "Use glw" ON)

    if(GR_USE_GLEW)
        find_package(GLEW REQUIRED)

        target_link_libraries(grender PRIVATE ${GLEW_LIBRARIES})
    endif()
    unset(GR_USE_GLEW CACHE)

    set(OpenGL_GL_PREFERENCE LEGACY)

    find_package(OpenGL REQUIRED)
    target_link_libraries(grender PRIVATE ${OPENGL_LIBRARIES})

else()
    find_library(EGL-lib EGL)
    find_library(GLESv3-lib GLESv3)

    target_link_libraries(grender PRIVATE ${log-lib} ${android-lib} ${EGL-lib} ${GLESv3-lib})
endif()

unset(GR_COMPILE_LINUX CACHE)
