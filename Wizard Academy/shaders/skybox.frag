#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D skyboxTexture;

void main()
{
    vec4 skyColor = texture(skyboxTexture, TexCoords);
    
    vec3 atmosphericTint = vec3(0.95, 0.97, 1.0); 
    skyColor.rgb *= atmosphericTint;
    
    
    skyColor.rgb *= 1.05;
    
    FragColor = skyColor;
}