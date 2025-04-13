#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // Having z equal w will always result in a depth of 1.0f
    gl_Position = projection * view * vec4(aPos, 1.0f);
    // We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
    TexCoords = aPos;
}