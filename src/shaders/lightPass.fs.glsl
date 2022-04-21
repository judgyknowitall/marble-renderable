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

uniform vec4 bg = vec4(0.2, 0.1, 0, 1);         // the background colour
uniform vec3 ambient = vec3(0.1, 0.07, 0.03);   // Ambient Light


uniform float near_plane = -5.f;
uniform float far_plane = 10.f;
uniform float sigma_t = 0.3f;

uniform vec3 k_schlick = vec3(0.04);


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
// Linearize Proj
/////////////////////////////////////////////////////////////////////

// required when using a perspective projection matrix
float linearizeDepth(float depth)
{
    //float z = depth * 2.0 - 1.0; // Back to NDC 
    float z = depth;
    float new_depth = (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
    //return (new_depth + 1)/ 2;
    return new_depth;
}


/////////////////////////////////////////////////////////////////////
// Fresnel Schlick Reflection Approximation
/////////////////////////////////////////////////////////////////////


vec3 fresnelSchlick(vec3 Normal, vec3 Diffuse)
{
    //vec3 F0 = k_schlick * Diffuse;
    //float cosTheta = max(dot(Normal, V), 0);
    //return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    float NdotL = max( 0, dot( Normal, L ) );
    return Diffuse + ( 1 - Diffuse ) * pow( ( 1 - NdotL ), 5 );
}


/////////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////////


void main()
{

    // check if we're a background pixel ASAP to save on computing
    float depth = texture(gDepth, TexCoords).x;
    if (depth == 1 && render_mode != 4) {
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
    //sss_depth = linearizeDepth(sss_depth) / far_plane;

    // SSS values
    float si = FragPos.z - LightPos.z;
    float sss_value = exp(-si * sigma_t);
    //Diffuse = vec3(1.0,0,0);

    // Render
    switch(render_mode) {
    
    case 0: // Phong
        phong(FragPos, Normal, Diffuse, Specular);
        FragColor = sss_value * FragColor;
        break;

    case 1: // Oren-Nayar
        orennayar(FragPos, Normal, Diffuse, Specular);
        FragColor = sss_value * FragColor;
        break;

    case 2:     // Subsurface Scattering
        FragColor = vec4(sss_value * Diffuse, 1.0);
        break;

    case 3:     // Reflection
        Diffuse = vec3(.1f, 0.1, 0.1);
        FragColor = vec4(fresnelSchlick(Normal, Diffuse),1.0);
        break;

    case 4: // Albedo
        FragColor = vec4( Diffuse, 1.0 );
        break;

    case 5: // Specular
        FragColor = vec4(Specular, Specular, Specular, 1.0);
        break;

    case 6: // Depth Map
        FragColor = vec4(vec3(sss_depth), 1.0);
        break;

    case 7:     // Position
        FragColor = vec4(FragPos, 1.0);
        break;
    
    case 8:     // Normal
        FragColor = vec4(Normal, 1.0);
        break;
    
    case 9:     // Depth
        float d2 = depth * depth;
        FragColor = vec4(d2 * d2, d2, depth, 1.0);
        break;

    default:    // screen space
        FragColor = vec4(TexCoords, 0.0, 1.0);		
    }
}