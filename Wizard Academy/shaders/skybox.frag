#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D skyboxTexture; // Use sampler2D because it's an OBJ model, not a CubeMap

void main()
{
    // Sample the texture directly. 
    // No lighting calculations needed.
    FragColor = texture(skyboxTexture, TexCoords);
}