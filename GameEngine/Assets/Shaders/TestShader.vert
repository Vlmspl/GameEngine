#version 460 core

layout(location = 0) in vec3 vPos;    // Vertex position
layout(location = 1) in vec2 vUv;     // Vertex texture coordinate
layout(location = 2) in vec3 vNormal; // Vertex normal

out vec2 fragUv;     // Passed to fragment shader
out vec3 fragPos;    // Passed to fragment shader
out vec3 fragNormal; // Passed to fragment shader

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    fragPos = vec3(uModel * vec4(vPos, 1.0));
    fragNormal = mat3(transpose(inverse(uModel))) * vNormal;
    fragUv = vUv;

    gl_Position = uProjection * uView * vec4(fragPos, 1.0);
}
