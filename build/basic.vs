//
//  basic.vs
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.
//  Input attributes recieved from the Mesh's VBO/VAO

#version 330 
layout (location = 0) in vec3 aPos;   //Vetex Position
layout (location = 1) in vec3 aColor;  //Vertex Color
//layout (location = 2) in vec2 aTexCoords;

/*
//Output variables passed to the Fragment Shader
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
 */

//Uniforms (which will be sending data to the amin C++ application)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;

void main()
{
    // Calculate the fragment's position in world space for lighting
    //FragPos = vec3(model*vec4(aPos,1.0));
    
    //Transform the normal vector to world space
    //Normal = mat3(transpose(inverse(model)))*aNormal;
    
    //Calculating the final screen position
    
    //Applying AVP transitions
    gl_Position = projection * view * model * vec4(aPos,1.0);
 
    ourColor = aColor;
    
}
