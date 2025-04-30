#version 330 core
in vec3 LocalPos;
out vec4 FragColor;

uniform sampler2D equirectangularMap;

const float PI = 3.14159265358979323846;

void main() {
    vec3 dir = normalize(LocalPos);
    float phi   = atan(dir.z, dir.x);
    float theta = acos(dir.y);
    vec2 uv = vec2((phi + PI) / (2.0 * PI), theta / PI);
    FragColor = texture(equirectangularMap, uv);
}
