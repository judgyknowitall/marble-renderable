#version 410 core

// Output
// depth is handled implicitly!
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gPosition;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D marbleTexture;


void main()
{
    // Store pos and normal to gBuffer
    gPosition = FragPos;
    gNormal = normalize(Normal);

    gColor = texture(marbleTexture, TexCoord);
    gColor = 1 - 0.5 * (1-gColor);
    // Create Marble Color from generated texture
    //vec4 stripes = vec4(236/256, 202/256, 164/256, 1.f);
    vec4 stripes = vec4(.1, .1, .1, 01);
    vec4 color = vec4(.98f, 0.875, 0.71, 1.f);
    //if (gColor.x < 0.99) gColor += stripes / (gColor.x+0.01);
    gColor += stripes;
    gColor.a = 0;
    
    //gColor = gColor * color;
}


