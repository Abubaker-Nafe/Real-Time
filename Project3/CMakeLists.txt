cmake_minimum_required(VERSION 3.17)
project(OpenGLProject)

set(CMAKE_C_STANDARD 99)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(opengl opengl.c)
target_link_libraries(opengl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
