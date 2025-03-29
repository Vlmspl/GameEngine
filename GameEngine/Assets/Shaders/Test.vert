#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 Uv;

void main() {
    Uv = aUv;
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0);
}
