//
//  basic.fs
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.

out vec4 FragColor;

//Input variables interpolated from the Vetex Shader
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//Uniforms
uniform sampler2D textureDiffuse1; //Primary texture
uniform vec3 lightColor;           // Moonlight color
uniform vec3 lightDirection;       // Direction of the light source

void main()
{
    //Soft Background Light -- Ambient Light
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    //The main directional moonlight -- Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);
    
    //Calculate diffuse factor -- max of dot product and 0
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;
    
    // Final Color Calculation
    vec4 texColor = texture(textureDiffuse1,TexCoords);
    vec3 result = (ambient + diffuse) * texColor.rgb;
    
    FragColor = vec4(result, texColor.a)
}

