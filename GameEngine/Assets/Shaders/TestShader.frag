#version 460 core

in vec2 uv;  // Input color from the vertex shader

out vec4 FragColor;  // Output color to the screen

uniform sampler2D u_texture;

void main()
{
    vec3 Color = texture(Texture, uv).rgb;

    // Set the fragment color based on the depth value
    FragColor = vec4(Color, 1.0);
}
