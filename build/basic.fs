//
//  basic.fs
//  Wizard Academy
//
//  Created by Diksha Chottani on 11/2/25.

#version 330
in vec3 ourColor;
out vec4 FragColor;


void main()
{
    
    FragColor =  vec4(ourColor, 1.0);
    
    /*
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
     */
}

