cmake_minimum_required(VERSION 3.0)

project(TIC_TAC_TOE C)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# "/home/sms/Загрузки/glfw-3.3/" - path to root of the glfw source directory downloaded and unpacked from https://www.glfw.org/download.html
add_subdirectory("/home/sms/Загрузки/glfw-3.3/" "build")

link_libraries(glfw)

include_directories(${glfw_INCLUDE_DIRS} "${GLFW_SOURCE_DIR}/deps")

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
            "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

add_executable(main main.c ${GLAD_GL})


