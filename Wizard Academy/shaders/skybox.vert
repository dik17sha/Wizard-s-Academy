#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // Not used, but kept for attribute alignment
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; // Technically not needed if we strip translation in View, but good for scaling

void main()
{
    TexCoords = aTexCoords;
    
    // 1. Standard transformation
    // Note: We cast view to mat3 to remove translation (movement), then back to mat4
    // This ensures the skybox doesn't move when you walk, only when you look around.
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    
    // 2. Force Z to W. 
    // After perspective division (z / w), the depth will be (w / w) = 1.0.
    // This pushes the skybox to the absolute farthest depth possible.
    gl_Position = pos.xyww;
}