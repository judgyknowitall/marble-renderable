#version 410 core

#define PI 3.1415926538

out vec4 FragColor;

in vec2 TexCoords;

// Input from gBuffer
uniform sampler2D gDepth;
uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;


// Uniforms
uniform vec3 L;     // Light Position
uniform vec3 V;     // View Position

uniform vec4 bg = vec4(0.2, 0.1, 0, 1);         // the background colour
uniform vec3 ambient = vec3(0.1, 0.07, 0.03);   // Ambient Light




/////////////////////////////////////////////////////////////////////
// Phong Shading
/////////////////////////////////////////////////////////////////////

void phong(vec3 FragPos, vec3 Normal, vec3 Diffuse, float Specular) {

    float specular_power = 81 * Specular * Specular;

    // Calculate R locally
    vec3 R = reflect(-L, Normal);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(Normal, L), 0.0) * Diffuse;

    // Write final color to the framebuffer
    if (Specular < 0.01)
        FragColor = vec4(ambient + diffuse, 1.0);
    else {
        vec3 specular = pow(max(dot(R, FragPos), 0.0), specular_power) * Specular * vec3(1, 1, 1);
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
}


/////////////////////////////////////////////////////////////////////
// Oren-Nayar
/////////////////////////////////////////////////////////////////////
// Yasuhiro Fujii's "improvement" is lightweight yet a pretty good match for
//  "full" Oren-Nayar. See https://mimosa-pudica.net/improved-oren-nayar.html

void orennayar(vec3 FragPos, vec3 Normal, vec3 Diffuse, float Specular) {

    float NdL = max(dot(Normal, L), 0);
    float NdV = max(dot(Normal, FragPos), 0);

    float s = max(dot(L, V), 0) - NdL * NdV;
    float t = 1;
    if (s > 0)
        t = max(NdL, NdV);

    float invA = 1. + (.5 - (2. / (PI * 3.))) * Specular;
    float B = Specular / invA;

    float factor = (1. / invA + B * s / t) * NdL;
    if ((factor > 1) && (NdL < 0.01))		// smooth out a math singularity
        FragColor = vec4(Diffuse * NdL, 1.0);
    else
        FragColor = vec4(Diffuse * factor, 1.0);
}



/////////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////////

void main()
{

    // check if we're a background pixel ASAP to save on computing
    float depth = texture(gDepth, TexCoords).x;
    if (depth == 1) {
        FragColor = bg;
        return;
    }

    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).xyz;
    vec3 Normal = texture(gNormal, TexCoords).xyz;
    vec3 Diffuse = texture(gColor, TexCoords).rgb;
    float Specular = texture(gColor, TexCoords).a;

    //phong(FragPos, Normal, Diffuse, Specular);
    orennayar(FragPos, Normal, Diffuse, Specular);

    // TESTS
    FragColor = vec4( Diffuse, 1.0 );	// albedo
    //FragColor = vec4(Specular, Specular, Specular, 1.0);	// roughness
    //FragColor = vec4(FragPos, 1.0);	// TEST COORDS
    //FragColor = vec4(Normal, 1.0);	    // view normal
    //FragColor = vec4(TexCoords, 0.0, 1.0);		// screen space

    float d2 = depth * depth;
    float d4 = depth * depth;
    //FragColor = vec4(d4 * d4, d4, depth, 1.0);	// depth value
}