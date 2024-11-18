#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
// layout (location = 2) in vec2 vTexCoord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat3 N;

out vec3 vertexPos;    // Untransformed position of the vertex (in object coordinates)
out vec3 fragPos;      // Transformed position of the vertex (in world coordinates)
out vec3 fragNormal;   // Transformed normal of the vertex (in world coordinates)
// out vec2 texCoord;

void main()
{
    vec4 transformed = M * vec4(vPosition, 1.0);
    gl_Position = P * V * transformed;

    vertexPos = vPosition;
    fragPos = transformed.xyz;
    fragNormal = N * vNormal;
}