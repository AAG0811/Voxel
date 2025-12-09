#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D albedo;

void main() {
    vec3 sunDir = normalize(vec3(0.4, 1.0, 0.0));
    vec3 sunCol = vec3(1.0, 0.95, 0.9);
    float sunInt = 0.8;
    vec3 ambient = vec3(0.1, 0.1, 0.15);

    // texcoords for diff tex
    vec2 tc = TexCoords * 0.5;
    if (Normal.y == 1.0) {
        // top grass
        tc -= 0.5;
    } else if (abs(Normal.x) == 1.0 || abs(Normal.z) == 1.0) {
        // side grass
        tc.x -= 0.5;
    }

    // diffuse attenuation
    float ndotl = max(dot(normalize(Normal), normalize(sunDir)), 0.0);

    vec3 baseCol = texture(albedo, tc+0.5).rgb;
    vec3 linCol = pow(baseCol, vec3(2.2));

    vec3 sunLight = sunCol * ndotl * sunInt;
    vec3 col = linCol * (ambient + sunLight);

    // correct gamma
    col = pow(col, vec3(1.0/2.2));
    FragColor = vec4(col, 1.0);
}
