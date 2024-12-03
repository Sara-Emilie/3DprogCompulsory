#version 330 core

out vec4 FragColor;

// Inputs from the vertex shader
in vec3 Normal;
in vec3 FragPos;
in vec3 Color;

// Uniforms
uniform vec3 viewPos;  // Camera position

void main()
{

    vec3 Up = vec3(0.0, 1.0, 0.0);

    // Normalize the normal
    vec3 norm = normalize(Normal);

    // Define constants for the light properties
    vec3 lightPos = viewPos;  // Light position same as view position
    vec3 lightColor = vec3(1.0, 1.0, 1.0);  // White light
    vec3 ambientColor = vec3(1.0, 1.0, 1.0);  // White ambient light
    float shininess = 100.0;  // Hardcoded shininess

    // Ambient lighting
    vec3 ambient = ambientColor * Color;

    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * Color;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // Final color
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
