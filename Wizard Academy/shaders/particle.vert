#version 330 core
layout (location = 0) in vec2 aPos;      
layout (location = 1) in vec2 aTexCoord; 

out vec2 TexCoords;
out vec3 WorldPos;

uniform vec3 particlePos;     
uniform vec3 cameraRight;     
uniform vec3 cameraUp;        
uniform float particleSize;   
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 vertexPosition = particlePos + cameraRight * aPos.x * particleSize + cameraUp * aPos.y * particleSize;
    
    vec4 viewPos = view * vec4(vertexPosition, 1.0);
    gl_Position = projection * viewPos;
    
    TexCoords = aTexCoord;
    WorldPos = vertexPosition;
}