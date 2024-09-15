#version 460 core

in vec3 fragColor;  // Input color from the vertex shader
in float CoordZ;

out vec4 FragColor;  // Output color to the screen

void main()
{
    // Map CoordZ from [-1, 1] to [0, 1]
    float depth = (CoordZ + 1.0) / 2.0;

    // Set the fragment color based on the depth value
    FragColor = vec4(fragColor, 1.0);
}
