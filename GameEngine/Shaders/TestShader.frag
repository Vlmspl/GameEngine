#version 330 core

in vec3 vertexColor; // Color passed from vertex shader

out vec4 FragColor;  // Output color to the screen

void main()
{
    // Set the fragment color based on the vertex color
    FragColor = vec4(vertexColor, 1.0);
}
