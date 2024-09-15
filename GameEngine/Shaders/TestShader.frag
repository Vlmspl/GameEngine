#version 460 core

in vec3 fragColor;  // Input color from the vertex shader

out vec4 FragColor;  // Output color to the screen

void main()
{
    // Set the fragment color based on the input color
    FragColor = vec4(fragColor, 1.0);  // Set alpha to 1.0 (opaque)
}
