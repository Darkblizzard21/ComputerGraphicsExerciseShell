#version 330 core

layout (location = 0) in vec3 aPos;      // Position
layout (location = 1) in vec3 aNormal;   // Normale
layout (location = 2) in vec2 aTexCoord; // UV-Koordinaten

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 crntPos;
out vec2 texCoord;

void main()
{
    // Position im Welt- und Clip-Raum
    vec4 worldPos = model * vec4(aPos, 1.0);
    crntPos = worldPos.xyz;

    // Normale korrekt transformieren (inverse transpose von model)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    texCoord = aTexCoord;

    gl_Position = projection * view * worldPos;
}
