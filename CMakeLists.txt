cmake_minimum_required(VERSION 3.0.0)

project(gr-window VERSION 0.1.0 LANGUAGES CXX C)

find_package(GLUT REQUIRED)
find_package(gr-render REQUIRED)
find_package(gr-math REQUIRED)

add_executable(gr-window main.cpp timer.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE gr-render/src)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GRRENDER_LIBRARIES} ${GRMATH_LIBRARIES})


