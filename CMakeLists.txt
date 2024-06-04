cmake_minimum_required(VERSION 3.0.0)

project(gr-window VERSION 0.1.0 LANGUAGES CXX C)

add_subdirectory(gr-render)


find_package(GLUT REQUIRED)

add_executable(gr-window main.cpp timer.cpp)


target_include_directories(${PROJECT_NAME} PRIVATE gr-render/src)

target_link_libraries(${PROJECT_NAME} PRIVATE grender ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})


