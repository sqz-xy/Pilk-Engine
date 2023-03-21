cmake_minimum_required(VERSION 3.0.0)
project(Pilk-Engine VERSION 0.1.0)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

link_directories(libs/glfw)
link_directories(libs/assimp)

include_directories(libs/glad)
include_directories(libs/KHR)
include_directories(libs/imgui)
include_directories(libs/glfw)
include_directories(libs/stb)
include_directories(libs/irrKlang)
include_directories(libs/glm)
include_directories(libs/assimp)
include_directories(include)

set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(Pilk-Engine ${SOURCES} "src/stb/stb_image.cpp" "src/Engine/Managers/SceneManager.cpp" "src/Engine/Managers/SceneManager.h"  "src/Engine/Scenes/Scene.h" "src/Engine/Objects/Geometry.h" "src/Engine/Objects/Geometry.cpp" "src/Engine/PilkEngineCommon.h" "src/Engine/Objects/Model.h" "src/Engine/Objects/Model.cpp")

find_package(OpenGL REQUIRED)
target_link_libraries(Pilk-Engine PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(Pilk-Engine PRIVATE glfw3 assimp)

file(COPY resources DESTINATION /)
