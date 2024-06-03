#include <iostream>

#include <gRender.h>

#include <GL/glut.h>

using namespace grr;

u32 m_width, m_height;

std::vector<Vertex2D> vertex2D {
    {Vector2(-0.5,  0.5)},
    {Vector2(-0.5, -0.5)},
    {Vector2( 0.5,  0.5)},
    {Vector2( 0.5,  0.5)},
    {Vector2(-0.5, -0.5)},
    {Vector2( 0.5, -0.5)}
};

void render() {
    Vector2 centerScreen(m_width / 2.0f, m_height / 2.0f);


    Matrix3x3 model =
        Math::translate<Matrix3x3>(centerScreen) *
        Math::rotate<Matrix3x3>(Quaternion::identity) *
        Math::scale<Matrix3x3>(Vector2(64, 64));

    std::vector<Vertex2D> vertices;
    
    for (const auto& v : vertex2D) {
        vertices.push_back({model * Vector3(v.position, 1.0f)});
    }

    gRender::SetRenderState(GR_BACKGROUND_COLOR, (void*)&gColor::black);
    gRender::SetRenderState(GR_BACKGROUND, GR_COLOR_BUFFER);

    gRender::Render2D();
    
    gRender::RenderPrimitive2D(PrimitiveType_Triangles, vertices.size(), vertices.data());

    glutSwapBuffers();
}

void WindowShape(int width, int height) {
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

    glutDisplayFunc(render);
    glutReshapeFunc(WindowShape);

    glutMainLoop();
    return 0;
}
