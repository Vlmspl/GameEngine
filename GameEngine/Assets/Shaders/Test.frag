#version 460 core

uniform sampler2D Texture;

out vec4 FragColor;

in vec2 Uv;
void main() {
    vec4 color = texture(Texture, Uv);
    FragColor = vec4(color.xyz, 1.0);
}
