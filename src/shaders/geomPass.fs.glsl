#version 410 core

// Output
// depth is handled implicitly!
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gPosition;

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D marbleTexture;
uniform int render_modifier = 0;


void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    // start writing it all out
    //gColor = Color;
    gColor = texture(marbleTexture, TexCoord);
    //gColor = vec4(TexCoord, 0.0, 1.0);

    //float c = marbleTexture();
    //gColor.rgb = vec3(c, c, c);
    //gColor.a = float(render_modifier) / 9.;		// ranges from 0 to 1
}


