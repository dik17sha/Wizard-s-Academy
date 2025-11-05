//
//  basic.vs
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//  Input attributes recieved from the Mesh's VBO/VAO

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//Output variables passed to the Fragment Shader
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

//Uniforms (which will be sending data to the amin C++ application)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate the fragment's position in world space for lighting
    FragPos = vec3(model*vec4(aPos,1.0));
    
    //Transform the normal vector to world space
    Normal = mat3(transpose(inverse(model)))*aNormal;
    
    //Calculating the final screen position
    glPosition = projection * view * vec4(aPos,1.0);
    
    TexCoords = aTexCoords;
}
