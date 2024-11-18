#version 430

layout (location = 0) in vec3 vPosition;

uniform mat4 P;
uniform mat4 V;     // Skybox view matrix (is mat4(mat3(V)) in the C++ code)

out vec3 texCoord;

void main() {
    texCoord = vPosition;
    gl_Position = P * V * vec4(vPosition, 1.0);
}