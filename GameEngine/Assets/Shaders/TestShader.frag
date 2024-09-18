#version 460 core

in vec2 fragUv;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

// Textures
uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uRoughness;

// Lighting parameters
const vec3 lightPos = vec3(4.0, 5.0, 10.0); // Light position
uniform vec3 viewPos;  // Camera/view position

void main()
{
    // Sample textures
    vec3 albedo = texture(uAlbedo, fragUv).rgb;
    vec3 normal = normalize(texture(uNormal, fragUv).rgb * 2.0 - 1.0); // Normal map transformation
    float roughness = texture(uRoughness, fragUv).r;

    // Lighting calculations
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Phong Lighting Model
    float ambientStrength = 0.2;
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 ambient = ambientStrength * vec3(1.0); // Fixed ambient color
    vec3 diffuse = diff * albedo;
    vec3 specular = spec * vec3(1.0); // White specular highlights

    // Combine lighting
    vec3 lighting = ambient + diffuse + specular;
    fragColor = vec4(albedo * lighting, 1.0); // Apply lighting to the albedo
}
