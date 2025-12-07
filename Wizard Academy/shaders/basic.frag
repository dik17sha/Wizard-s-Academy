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
    vec3 materialAmbient = vec3(0.28);       
    float specularStrength = 0.5;           
    float rimStrength = 0.35;                
    
    vec3 norm = normalize(Normal);
    vec3 lightDirN = normalize(-lightDirection); 
    vec3 viewDir = normalize(viewPos - FragPos);
    
    float ao = 0.4 + 0.6 * max(dot(norm, vec3(0.0, 1.0, 0.0)), 0.0);
    vec3 ambient = materialAmbient * lightColor * ao;
    
    float diffuseWrap = 0.3; 
    float diff = max(dot(norm, lightDirN), 0.0);
    float wrappedDiff = (diff + diffuseWrap) / (1.0 + diffuseWrap);
    vec3 diffuse = wrappedDiff * lightColor;
    
    vec3 halfwayDir = normalize(lightDirN + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor; 
    

    float rimDot = 1.0 - max(dot(viewDir, norm), 0.0);
    float rimIntensity = pow(rimDot, 3.5); 
    vec3 rimColor = vec3(0.5, 0.6, 0.8) * lightColor; 
    vec3 rim = rimStrength * rimIntensity * rimColor;
    
  
    vec3 warmColor = vec3(1.0, 0.95, 0.85);   
    vec3 coolColor = vec3(0.7, 0.75, 0.9);    
    float toneBlend = diff * 0.5 + 0.5;       
    vec3 tonedLight = mix(coolColor, warmColor, toneBlend);
    

    float fogStart = 50.0;
    float fogEnd = 300.0;
    vec3 fogColor = vec3(0.6, 0.7, 0.85); 
    
    float distanceToCamera = length(viewPos - FragPos);
    float fogFactor = smoothstep(fogStart, fogEnd, distanceToCamera);
    fogFactor = clamp(fogFactor, 0.0, 0.75); 
    
    vec3 lightingResult = (ambient + diffuse * tonedLight + specular + rim);
    
    vec3 materialColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 finalColor = lightingResult * materialColor;
    
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    
    finalColor = mix(finalColor, fogColor, fogFactor);
    
    FragColor = vec4(finalColor, 1.0);
}
