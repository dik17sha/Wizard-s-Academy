#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;
uniform vec3 lightDirection; 
uniform vec3 lightColor;    
uniform float shininess;     

void main()
{    
    vec3 materialAmbient = vec3(0.2); 
    
    vec3 norm = normalize(Normal);
    vec3 lightDirN = normalize(-lightDirection); 
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = materialAmbient * lightColor;

    float diff = max(dot(norm, lightDirN), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 halfwayDir = normalize(lightDirN + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor * 5.0f; 

    vec3 lightingResult = ambient + diffuse + specular;

    vec3 materialColor = texture(texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(lightingResult * materialColor, 1.0);
}