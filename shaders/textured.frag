#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D albedo;

void main() {
    // modify texture coords
    vec2 tc = TexCoords * 0.5;
    // if (Normal.y == 1.0) {
    //     FragColor = vec4(texture(albedo, tc).rgb, 1.0);
    // } else if (Normal.y == -1.0) {
    //     FragColor = vec4(texture(albedo, tc+0.5).rgb, 1.0);
    // } else {
    //     FragColor = vec4(texture(albedo, vec2(tc.x, tc.y+0.5)).rgb, 1.0);
    // }
    // dirt texture
    if (Normal.y == 1.0) {
        // top grass
        tc -= 0.5;
    } else if (abs(Normal.x) == 1.0 || abs(Normal.z) == 1.0) {
        // side grass
        tc.x -= 0.5;
    }
    vec3 texcol = texture(albedo, tc+0.5).rgb;
    texcol = pow(texcol, vec3(2.2));
    vec3 ambient = vec3(1.0);
    if (Normal.x == 1.0) {
        ambient = vec3(0.9);
    } else if (Normal.z == -1.0) {
        ambient = vec3(0.7);
    } else if (Normal.x == -1.0) {
        ambient = vec3(0.7);
    } else if (Normal.z == 1.0) {
        ambient = vec3(0.9);
    }
    vec3 col = texcol * ambient;
    col = pow(col, vec3(1/2.2));
    FragColor = vec4(col, 1.0);
    // FragColor = vec4(TexCoords, 0.0, 1.0);
}
