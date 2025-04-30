#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    if (length(norm) < 0.01) {
    norm = vec3(0.0, 1.0, 0.0); // Fallback für kaputte Daten
    }

    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = diffuse;
    FragColor = vec4(result, 1.0);
}
