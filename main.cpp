#include "timer.h"

#include <iostream>

#include <gTexture.h>
#include <gRender.h>
#include <gShader.h>

#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace grr;

u32 m_width, m_height;

Time timer(60.0f);

float r =0;

Vector3 forward;
Vector3 right;

Vector3 camPos(0.0f, 0.0f, -5.0f);

void Keyboard(unsigned char key, int x, int y) {
}

Vector2 mousePos;
Vector2 startMousePos;

void motion(int x, int y) {
}

void MouseButton(int button, int state, int x, int y) {
}

std::vector<Vertex3D> vertex3D {
    {Vector3(-1.0f, -1.0f,  1.0f)},
    {Vector3(-1.0f,  1.0f,  1.0f)},
    {Vector3(-1.0f, -1.0f, -1.0f)},
    {Vector3(-1.0f,  1.0f, -1.0f)},
    {Vector3( 1.0f, -1.0f,  1.0f)},
    {Vector3( 1.0f,  1.0f,  1.0f)},
    {Vector3( 1.0f, -1.0f, -1.0f)},
    {Vector3( 1.0f,  1.0f, -1.0f)}
};

std::vector<u32> indices3D {
    1, 2, 0,
    3, 6, 2,
    7, 4, 6,
    5, 0, 4,
    6, 0, 2,
    3, 5, 7,
    1, 3, 2,
    3, 7, 6,
    7, 5, 4,
    5, 1, 0,
    6, 4, 0,
    3, 1, 5
};

float ms = 0;
static int itemCount = 0;

std::vector<Vertex2D> vertex2D {
    {Vector2(-0.5,  0.5), Vector2(0.0, 1.0)},
    {Vector2(-0.5, -0.5), Vector2(0.0, 0.0)},
    {Vector2( 0.5,  0.5), Vector2(1.0, 1.0)},
    {Vector2( 0.5,  0.5), Vector2(1.0, 1.0)},
    {Vector2(-0.5, -0.5), Vector2(0.0, 0.0)},
    {Vector2( 0.5, -0.5), Vector2(1.0, 0.0)} 
};

Matrix3x3 model = Matrix4x4::identityMatrix;


gTexture* texture = nullptr;

void init() {
    Vector2 screenSize(m_width, m_height);  

    int x, y, ch;
    void* pixels = stbi_load("/home/grsource/Downloads/Sponza/VasePlant_diffuse.png", &x, &y, &ch, 0);

    std::cout << ch << std::endl;

    texture = gTexture::Create(x, y, TextureFormat_RGBA, pixels);

    stbi_image_free(pixels);
}

void render() {
    Time::deltaTime = timer.SleepTick();

    Vector2 screenSize(m_width, m_height);  

    std::vector<Vertex2D> vertices;

    static float r = 0;
    r += 95.0f * Time::deltaTime;

    r = std::fmod(r, 360.0f);

    model = Math::translate<Matrix3x3>(screenSize / 2.0f) *
        Math::rotate<Matrix3x3>(Math::euler({0.0f, 0.0f, Math::rad(r)})) *
        Math::scale<Matrix3x3>(Vector2(r / 3, r / 3));

    for (const auto& v : vertex2D) {
        vertices.push_back({Vector2(model * Vector3(v.position, 1.0f)), v.uv});
    }

    gRender::SetRenderState(GR_BACKGROUND_COLOR, (void*)&gColor::black);
    gRender::SetRenderState(GR_BACKGROUND, GR_COLOR_BUFFER);
    gRender::SetRenderState(GR_VIEWPORT, (void*)&screenSize);
    gRender::SetRenderState(GR_CULL_FACE, GR_TRUE);
    gRender::SetRenderState(GR_CULL, GR_BACK);

    gRender::Render2D();

    if (texture->isValid()) {
        u32 textureIndex = 0;

        texture->bind();
        gShader::SetUniform("u_texture", reinterpret_cast<const int*>(&textureIndex));
    }

    gRender::RenderPrimitive2D(PrimitiveType_Triangles, vertices.size(), vertices.data());

    glutSwapBuffers();
}

void framebufferSize(int width, int height) {
    m_width = width;
    m_height = height;
    gRender::SetResolution(width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL com GLUT");

    if (!gRender::OpenGLInit()) {
        std::cout << "Falied opennable gr-render" << std::endl;
        return 1;
    }

    init();

    // glutDisplayFunc(render);

    glutMotionFunc(motion);
    glutMouseFunc(MouseButton);
    glutKeyboardFunc(Keyboard);
    glutReshapeFunc(framebufferSize);
    glutIdleFunc(render);

    glutMainLoop();
    return 0;
}
    