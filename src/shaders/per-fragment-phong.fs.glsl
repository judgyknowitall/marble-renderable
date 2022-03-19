#version 410 core

// Output
layout (location = 0) out vec4 color;

// Input from vertex shader
in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

// Material properties
uniform vec3 k_d = vec3(172.0/255.0, 227.0/255.0, 232.0/255.0);
uniform vec3 k_s = vec3(0.5);
uniform float specular_power = 60.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

void main(void)
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(N, L), 0.0) * k_d;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * k_s;

    // Write final color to the framebuffer
    //color = vec4(ambient + diffuse + specular, 1.0);

    color = vec4(1.0f, 0.5f, 0.2f, 1.0f); //TODO
}
