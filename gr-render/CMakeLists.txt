project("gr-render")

set(render_src
    src/gTexture.cpp
    src/gRender.cpp
    src/gShader.cpp
    src/gl.cpp
)

set(render_hdr
    src/gTexture.h
    src/gRender.h
    src/gShader.h
    src/common.h
    src/gColor.h
    src/types.h
)

add_library(grender STATIC ${render_src} ${render_hdr})

option(GR_COMPILE_LINUX "Compile on Android" OFF)

find_package(gr-math REQUIRED)

target_link_libraries(grender PRIVATE ${GRMATH_LIBRARIES})

if (NOT GR_COMPILE_LINUX)
    option(GR_USE_GLEW "Use glw" ON)

    if (GR_USE_GLEW)
        find_package(GLEW REQUIRED)

        target_link_libraries(grender PRIVATE ${GLEW_LIBRARIES})
    endif (GR_USE_GLEW)

    set(OpenGL_GL_PREFERENCE LEGACY)

    find_package(OpenGL REQUIRED)
    target_link_libraries(grender PRIVATE ${OPENGL_LIBRARIES})

else (NOT GR_COMPILE_LINUX)
    find_library(EGL-lib EGL)
    find_library(GLESv3-lib GLESv3)

    target_link_libraries(grender PRIVATE ${log-lib} ${android-lib} ${EGL-lib} ${GLESv3-lib})
endif (NOT GR_COMPILE_LINUX)


