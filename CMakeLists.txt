cmake_minimum_required(VERSION 3.20)
project(BlockWorld)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TARGET blockworld)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# STAGE 1: CREATING THE EXECUTABLE
# Add C++ files to executable
file(GLOB_RECURSE CPPS "${SRC}/*.cpp" "${SRC}/*.h")

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Set OpenGL_GL_PREFERENCE explicitly
set(OpenGL_GL_PREFERENCE "GLVND")

# Locating libraries
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLEW)

# OpenGL is required
IF (OPENGL_FOUND)
    message(STATUS "OpenGL sources found")
ELSE()
    message(FATAL_ERROR "OpenGL not found")
ENDIF ()

# If GLEW is not found, add sources from library files
IF (GLEW_FOUND)
    message(STATUS "GLEW sources found")
ELSE()
    message(STATUS "GLEW not found. Adding sources from lib/glew-2.1.0/build/cmake...")
    add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glew-2.1.0/build/cmake)
ENDIF ()

# Add GLFW sources from library files
message(STATUS "Adding GLFW3 sources from lib/glfw-3.3.8...")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.8)

# Add header-only libraries
include_directories(${PROJECT_SOURCE_DIR}/lib/fastnoiselite)
include_directories(${PROJECT_SOURCE_DIR}/lib/tinyobjloader)
include_directories(${PROJECT_SOURCE_DIR}/lib/stbimage)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui-1.90)

# Add ImGuiFileDialog sources from library files
message(STATUS "Adding ImGuiFileDialog sources from lib/ImGuiFileDialog-0.6.6.1...")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ImGuiFileDialog-0.6.6.1)

# Add CSLPQ (Priority Queue) sources from library files
message(STATUS "Adding CSLPQ sources from lib/CSLPQ...")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/CSLPQ)

# Add spdlog sources from library files
message(STATUS "Adding spdlog sources from lib/spdlog...")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/spdlog)

# Create executable with all sources
file(GLOB IMGUI_SOURCES
        "lib/imgui-1.90/*.cpp"
        "lib/imgui-1.90/*.h"
        "lib/imgui-1.90/backends/imgui_impl_glfw.cpp"
        "lib/imgui-1.90/backends/imgui_impl_glfw.h"
        "lib/imgui-1.90/backends/imgui_impl_opengl3.cpp"
        "lib/imgui-1.90/backends/imgui_impl_opengl3.h"
)
add_executable(${TARGET} ${CPPS} ${IMGUI_SOURCES})

# STAGE 2: LINKING LIBRARIES
# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -std=c++17")

# Link OpenGL
IF (OPENGL_FOUND)
    message(STATUS "Linking OpenGL from system (" ${OPENGL_INCLUDE_DIR} ") (" ${OPENGL_LIBRARIES} ")")
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})
ELSE ()
    message(FATAL_ERROR "OpenGL not found")
ENDIF ()

# Link GLEW
IF (GLEW_FOUND)
    message(STATUS "Linking GLEW from system (" ${GLEW_INCLUDE_DIRS} ") (" ${GLEW_LIBRARIES} ")")
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${GLEW_LIBRARIES})
ELSE ()
    message(STATUS "Linking GLEW from lib/glew-2.1.0/build/cmake...")
    target_include_directories(${TARGET} PRIVATE
            ${PROJECT_SOURCE_DIR}/lib/glew-2.1.0/include
    )
    target_link_libraries(${TARGET} glew_s)
ENDIF ()

# Link GLFW
message(STATUS "Linking GLFW...")
target_link_libraries(${TARGET} glfw)

# Link ImGui
message(STATUS "Linking ImGui from lib/imgui-1.90...")
target_include_directories(${TARGET} PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90/backends
)

# Link ImGuiFileDialog
message(STATUS "Linking ImGuiFileDialog")
target_link_libraries(${TARGET} ImGuiFileDialog)

# Link CSLPQ
message(STATUS "Linking CSLPQ...")
target_link_libraries(${TARGET} CSLPQ)

# Link spdlog
message(STATUS "Linking spdlog from lib/spdlog...")
target_link_libraries(${TARGET} spdlog::spdlog)

# Link OS-specific libraries
if (WIN32)
    message(STATUS "Linking Windows libraries")
    target_link_libraries(${TARGET} opengl32 gdi32 user32 kernel32)
else()
    message(STATUS "Linking Linux libraries")
    target_link_libraries(${TARGET} ${CMAKE_DL_LIBS} Xext X11)
endif()