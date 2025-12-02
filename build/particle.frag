#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;

uniform vec4 particleColor;

void main()
{
    vec2 center = vec2(0.5);
    float dist = length(TexCoords - center);
    
    float core = 1.0 - smoothstep(0.0, 0.15, dist);
    core = pow(core, 1.5);
    
    float midGlow = 1.0 - smoothstep(0.0, 0.35, dist);
    midGlow = pow(midGlow, 2.0);
    
    float outerGlow = 1.0 - smoothstep(0.0, 0.5, dist);
    outerGlow = pow(outerGlow, 3.0);
    
    float glowIntensity = core * 3.0 + midGlow * 1.5 + outerGlow * 0.5;
    
    vec3 finalColor = particleColor.rgb * glowIntensity;
    
    float alpha = particleColor.a * (core * 0.9 + midGlow * 0.6 + outerGlow * 0.3);
    
    FragColor = vec4(finalColor, alpha);
}
