#version 460 core

layout(location = 0) in vec3 vPos;  // Vertex position attribute
layout(location = 1) in vec3 vUv;  // Vertex color attribute

uniform float u_time;    // Time uniform
uniform mat4 u_projection; // Projection matrix uniform
uniform mat4 u_view; // View matrix uniform

out vec2 uv;  // Output color to the fragment shader

void main()
{
    // Pass the vertex color to the fragment shader
    uv = vUv;

    vec3 offset = vec3(0, 0, -2);

    // Set the vertex position
    gl_Position = u_projection * u_view * vec4(vPos, 1.0);

}
