#version 330 core

in vec3 Normal;
in vec3 crntPos;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 lightColor;

uniform sampler2D tex0; // diffuse texture
uniform sampler2D tex1; // specular texture map

out vec4 FragColor;

void main()
{
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 3.0;
    float b = 0.7;
    float inten = 1.0 / (a * dist * dist + b * dist + 1.0);

    // ambient lighting
    float ambient = 0.20;

    // diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0);

    // specular lighting
    float specularLight = 0.50;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
    float specular = specAmount * specularLight;

    vec3 texColor = texture(tex0, texCoord).rgb;
    float specMap = texture(tex1, texCoord).r;

    vec3 result = texColor * (diffuse + ambient) + vec3(specular * specMap * inten) * lightColor;

    FragColor = vec4(result, 1.0);
}
