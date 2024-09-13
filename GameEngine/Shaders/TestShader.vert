#version 330 core

layout (location = 0) in vec3 aPos; // Vertex position

out vec3 vertexColor; // Output color for fragment shader

void main()
{
    // Pass the position as the color (normalized between -1 and 1)
    vertexColor = (aPos + vec3(1.0)) * 0.5;
    
    // Set the vertex position
    gl_Position = vec4(aPos, 1.0);
}
