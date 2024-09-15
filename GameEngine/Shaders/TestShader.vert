#version 460 core

layout(location = 0) in vec3 vPos;  // Vertex position attribute
layout(location = 1) in vec3 vCol;  // Vertex color attribute

uniform float u_time; // Time uniform

out vec3 fragColor;  // Output color to the fragment shader

void main()
{
    // Pass the vertex color to the fragment shader
    fragColor = vCol;

    vec3 offset = vec3(0, sin(u_time+vPos.x)*0.5, 0);

    // Set the vertex position
    gl_Position = vec4(vPos + offset, 1.0);
}
