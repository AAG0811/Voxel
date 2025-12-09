#version 330 core
out vec4 FragColor;

in vec3 TexCoords;


// uniform samplerCube skybox;

void main()
{    
    // // FragColor = texture(skybox, TexCoords);
    // vec3 col = vec3(TexCoords.y * 0.5 + 0.6) * vec3(0.3, 0.5, 0.9);
    // col + vec3(0.2) * vec3(0.2, 0.3, 0.8);
    // // col = mix(vec3(0.3, 0.5, 0.9), vec3(0.1, 0.3, 0.7), TexCoords.y);

    float timeOfDay = 1.0; // 0 - 1
    vec3 sunDir = normalize(vec3(0.4, 1.0, 0.0));

    float sunAngle = dot(normalize(TexCoords), sunDir);

    // interpolate day/night colors 
    vec3 dayCol = vec3(0.53, 0.81, 1.0);
    vec3 nightCol = vec3(0.6, 0.7, 0.8);

    vec3 skyCol = mix(nightCol, dayCol, sunAngle);

    FragColor = vec4(skyCol, 1.0);
}