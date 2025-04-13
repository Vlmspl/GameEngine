#version 460 core

out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skyboxTexture; // Regular cubemap texture sampler

void main() {
    // Use the skyTexture sampler to sample the texture
    //vec3 color = texture(skyboxTexture, TexCoords).xyz;
    //FragColor = vec4(color, 1.0);
    FragColor = vec4((TexCoords+1.0)/2, 1.0);
}
