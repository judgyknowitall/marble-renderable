#version 410 core

// Output
// depth is handled implicitly!
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gPosition;
layout(location = 3) out vec4 gLightSpacePos;

in vec4 LightFragPos;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D marbleTexture;



void main()
{
    // Store pos and normal to gBuffer
    gLightSpacePos = LightFragPos;
    gPosition = FragPos;
    gNormal = normalize(Normal);

    float marble_c = texture(marbleTexture, TexCoord).r;
    //gColor.rgb = vec3(marble_c);

    vec4 color = vec4(.88f, 0.775, 0.61, 1.f);
    gColor = 1 - (1-marble_c) * (1-color);      // Adjust Transparency
    // Create Marble Color from generated texture
    //vec4 stripes = vec4(236/256, 202/256, 164/256, 1.f);
    vec4 stripes = vec4(.1, .1, .1, 1);
    
    //gColor += color;
    //gColor = 1 - (1-gColor) * (1-color);

    //gColor *= vec4(0.98, 0.977, 0.926,1.f);
    gColor.a = 0;
    
    //gColor = gColor * color;
}


