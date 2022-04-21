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
uniform vec3 marble_base_clr;


void main()
{
    // Store pos and normal to gBuffer
    gLightSpacePos = LightFragPos;
    gPosition = FragPos;
    gNormal = normalize(Normal);

    float marble_tex = texture(marbleTexture, TexCoord).r;
    //gColor.rgb = vec3(marble_tex);              // Just B&W texture

    gColor.rgb = 1 - (1-marble_tex) * (1-marble_base_clr);      // Adjust Transparency
    // Create Marble Color from generated texture
    //vec4 stripes = vec4(236/256, 202/256, 164/256, 1.f);
    vec4 stripes = vec4(.1, .1, .1, 1);

    //gColor *= vec4(0.98, 0.977, 0.926,1.f);
    gColor.a = 0;
}


