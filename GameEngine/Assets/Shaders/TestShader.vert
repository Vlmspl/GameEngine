#version 460 core

layout(location = 0) in vec3 vPos;   // Vertex position attribute
layout(location = 1) in vec2 vUv;    // Vertex UV attribute
layout(location = 2) in vec3 vNormal; // Vertex normal attribute

uniform mat4 u_model;     // Model matrix uniform
uniform mat4 u_view;      // View matrix uniform
uniform mat4 u_projection; // Projection matrix uniform

out vec2 uv;      // Output UV to the fragment shader
out vec3 fragPos; // Output fragment position to the fragment shader
out vec3 normal;  // Output normal to the fragment shader

void main()
{
    // Pass the UV coordinates and normal to the fragment shader
    uv = vUv;

    // Transform vertex position to world space
    vec4 worldPos = u_model * vec4(vPos, 1.0);
    fragPos = worldPos.xyz;
    
    // Transform normal to world space
    normal = normalize(mat3(transpose(inverse(u_model))) * vNormal);

    // Set the vertex position in clip space
    gl_Position = u_projection * u_view * worldPos;
}
