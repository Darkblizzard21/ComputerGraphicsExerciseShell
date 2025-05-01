#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture_diffuse;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    vec3 color = texture(texture_diffuse, TexCoords).rgb;

    // Phong Shading
    vec3 ambient = 0.1 * color;

    vec3 norm = normalize(Normal);
    vec3 lightDirN = normalize(-lightDir);
    float diff = max(dot(norm, lightDirN), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirN, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * lightColor * 0.3;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
