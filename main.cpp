#include "timer.h"

#include <iostream>
#include <fstream>

#include <gr-render/gRender.h>
#include <gr-render/gShader.h>

#include <GL/glut.h>


using namespace grr;

u32 m_width, m_height;

Time timer(60.0f);

float r =0;

Vector3 forward;
Vector3 right;

Vector3 camPos(0.0f, 0.0f, -5.0f);

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
       camPos += forward * 10.0f * Time::deltaTime;
    }
    if (key == 's') {
        camPos -= forward * 10.0f * Time::deltaTime;
    }
    if (key == 'a') {
        camPos += right * 10.0f * Time::deltaTime;
    }
    if (key == 'd') {
        camPos -= right * 10.0f * Time::deltaTime;
    }

    // glutPostRedisplay();
}

Vector2 mousePos;
Vector2 startMousePos;

void motion(int x, int y) {
    Vector2 curr((float)x, (float)y);

    Vector2 direction = Math::normalize(curr - startMousePos);

    mousePos += direction * 1.1f;

    mousePos.y = std::max(std::min(mousePos.y, 89.0f), -89.0f);
    mousePos.x = std::fmod(mousePos.x, 360.0f);
    startMousePos = curr;
}

void MouseButton(int button, int state, int x, int y) {
    if (button == 0 && state == 0) {
        startMousePos = {(float)x, (float)y};
    }
}

std::vector<gVertex3D> vertex3D {
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

gShader* m_shader = nullptr;

const std::string getFileContent(const std::string& path) {
    std::ifstream is(path);

    std::string content((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    is.close();
    return content;
}

void init() {
    auto frag = getFileContent("../res/shader/default.frag");
    auto vert = getFileContent("../res/shader/default.vert");

    m_shader = gShader::Create({frag.c_str()}, {vert.c_str()});

    std::cout << frag << std::endl;
}

void render() {
    Time::deltaTime = timer.SleepTick();

    Vector2 screenSize(m_width, m_height);  

    Quaternion rotation = Math::normalize(Math::euler({Math::rad(mousePos.y), Math::rad(-mousePos.x), 0.0f}));

    forward = rotation * Vector3::forward;
    right = rotation * Vector3::right;
    Vector3 up = rotation * Vector3::up;

    Matrix4x4 projection = Math::perspective(Math::rad(90.0f), ((float)m_width / m_height), 1000.0f, 0.01f);
    Matrix4x4 view = Math::lookAt(camPos, camPos + forward, up);

    Matrix4x4 model = 
        Math::translate<Matrix4x4>(Vector3::zero) *
        Math::rotate<Matrix4x4>(Quaternion::identity) *
        Math::scale<Matrix4x4>(Vector3::one);

    gRender::SetRenderState(GR_BACKGROUND_COLOR, (void*)&gColor::black);
    gRender::SetRenderState(GR_BACKGROUND, GR_COLOR_BUFFER | GR_DEPTH_BUFFER);
    gRender::SetRenderState(GR_VIEWPORT, (void*)&screenSize);
    gRender::SetRenderState(GR_DEPTH, GR_TRUE);
    gRender::SetRenderState(GR_CULL_FACE, GR_TRUE);
    gRender::SetRenderState(GR_CULL, GR_BACK);

    m_shader->bind();

    gShader::SetUniform("projection", 1, projection);
    gShader::SetUniform("view", 1, view);

    gShader::SetUniform("model", 1, model);

    gRender::RenderPrimitive3D(PrimitiveType_Triangles, vertex3D.size(), vertex3D.data(), indices3D.size(), indices3D.data());

    glutSwapBuffers();
    glutPostRedisplay();
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

    glutDisplayFunc(render);
    glutMotionFunc(motion);
    glutMouseFunc(MouseButton);
    glutKeyboardFunc(Keyboard);
    glutReshapeFunc(framebufferSize);
    // glutIdleFunc(render);

    glutMainLoop();
    return 0;
}
    