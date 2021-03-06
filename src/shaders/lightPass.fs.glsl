#version 410 core


#define PI 3.1415926538


out vec4 FragColor;

in vec2 TexCoords;


// Input from gBuffer
uniform sampler2D gDepth;
uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gLightSpacePos;

// Shadow Map
uniform sampler2D depthMap;


// Uniforms
uniform vec3 L;     // Light Position
uniform vec3 V;     // View Position

uniform int render_mode = 2;
uniform bool sss_first = false;

uniform vec4 bg = vec4(0.2, 0.1, 0, 1);         // the background colour
uniform vec3 ambient = vec3(0.1, 0.07, 0.03);   // Ambient Light


uniform float near_plane = -5.f;
uniform float far_plane = 10.f;
uniform float sigma_t = 0.3f;

uniform float k_schlick = 1.0;


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
// Fresnel Schlick Reflection Approximation
/////////////////////////////////////////////////////////////////////


vec3 fresnelSchlick(vec3 Normal, vec3 FragPos, vec3 Diffuse)
{
    vec3 V_n = normalize(FragPos);
    vec3 L_n = L;
    vec3 H = normalize(V_n + L_n);
    float cosTheta = max(0, dot(H,Normal));
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Diffuse, 0.1); // metalness = 0

    //float cosTheta = max( 0, dot( Normal, L ) );
    vec3 schlick = F0 + ( 1 - F0 ) * pow( 1 - cosTheta , 5 );

    //Y = 255 - P*(255-X) // P = opacity
    return Diffuse - k_schlick * (1-schlick);
}


/////////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////////


void main()
{

    // check if we're a background pixel ASAP to save on computing
    float depth = texture(gDepth, TexCoords).x;
    if (depth == 1 && render_mode != 6) {
        FragColor = bg;
        return;
    }

    // retrieve data from gbuffer
    vec4 LightPos = texture(gLightSpacePos, TexCoords);
    vec3 FragPos = texture(gPosition, TexCoords).xyz;
    vec3 Normal = texture(gNormal, TexCoords).xyz;
    vec3 Diffuse = texture(gColor, TexCoords).rgb;
    float Specular = texture(gColor, TexCoords).a;
    float sss_depth = texture(depthMap, TexCoords).x;

    // SSS values
    float si = FragPos.z - LightPos.z;
    float sss_value = exp(-si * sigma_t);

    // Render
    switch(render_mode) {

    case 0:     // Phong
        phong(FragPos, Normal, Diffuse, Specular);
        break;


    case 1:     // Oren-Nayar
        orennayar(FragPos, Normal, Diffuse, Specular);
        break;

    case 2:     // Marble Rendering
        if (sss_first) {
            vec3 sss_color = sss_value * Diffuse; // SSS
            FragColor = vec4(fresnelSchlick(Normal, FragPos, sss_color),1.0); // Reflection
        }
        else {
            vec3 refl = fresnelSchlick(Normal, FragPos, Diffuse);
            FragColor = vec4(sss_value * refl, 1.0);
        }
        break;

    case 3:     // Subsurface Scattering
        FragColor = vec4(sss_value * Diffuse, 1.0);
        break;

    case 4:     // Reflection
        Diffuse = vec3(.1f, 0.1, 0.1);
        FragColor = vec4(fresnelSchlick(Normal, FragPos, Diffuse),1.0);
        break;

    case 5:     // Albedo
        FragColor = vec4( Diffuse, 1.0 );
        break;

    case 6:     // Shadow Map
        FragColor = vec4(vec3(sss_depth), 1.0);
        break;

    case 7:     // Depth
        float d2 = depth + depth;
        FragColor = vec4(d2 * d2, d2, depth, 1.0);
        break;

    case 8:     // Position
        FragColor = vec4(FragPos, 1.0);
        break;
    
    case 9:     // Normal
        FragColor = vec4(Normal, 1.0);
        break;

    default:    // screen space
        FragColor = vec4(TexCoords, 0.0, 1.0);		
    }
}