#version 460 core

in vec2 uv;          // Input UV coordinates from the vertex shader
in vec3 fragPos;     // Input fragment position from the vertex shader
in vec3 normal;      // Input normal from the vertex shader

out vec4 FragColor;  // Output color to the screen

uniform sampler2D u_texture; // Texture sampler
uniform vec3 u_viewPos;      // Camera position uniform

// Material properties
uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

// Light properties
uniform struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

void main()
{
    // Texture color
    vec3 albedo = texture(u_texture, uv).rgb;

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // Final color
    vec3 result = ambient + diffuse + specular;
    vec3 color = albedo * result; // Combine texture color with Phong lighting
    FragColor = vec4(color, 1.0);
}
