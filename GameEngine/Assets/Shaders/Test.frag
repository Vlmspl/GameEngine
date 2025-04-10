#version 460 core

uniform sampler2D Texture;  // Normal map texture

out vec4 FragColor;

in vec2 Uv;
in vec3 Normal;  // Surface normal

void main() {
    // Fetch the normal from the normal map (ensure it's in the correct space)
    vec3 normalMap = normalize(texture(Texture, Uv).xyz * 2.0 - 1.0); // Normalized in the range [-1, 1]

    // Combine the original normal with the normal from the normal map
    vec3 combinedNormal = normalize(normalize(Normal) + normalMap); // Blend the normals

    // For visualization, use the combined normal as color
    FragColor = vec4((combinedNormal + 1.0) * 0.5, 1.0); // Visualize normal in the [0, 1] range
}
