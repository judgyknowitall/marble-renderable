#version 410 core

// Output
// depth is handled implicitly!
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gPosition;

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 Color;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

uniform int render_modifier = 0;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    //float sinCol = 255 * sin(FragPos.x + FragPos.y);
    //gColor.rgb = vec3(sinCol, sinCol, sinCol);


    // start writing it all out
    //gColor.rgb = vec3(0.667, 0.890, 0.910);		// TODO: texture mapping!
    gColor.a = float(render_modifier) / 9.;		// ranges from 0 to 1

    gColor.rgb = Color.rgb;

    // and the diffuse per-fragment color
    //gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}