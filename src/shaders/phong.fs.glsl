#version 410 core


// Inputs
in vec3 normal_dir;
in vec3 light_dir;
in vec3 view_dir;


// Output
layout (location = 0) out vec4 color;


// Material properties
uniform vec3 k_d = vec3(172.0/255.0, 227.0/255.0, 232.0/255.0);
uniform vec3 k_s = vec3(0.5);
uniform float specular_power = 60.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);


void main(void)
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(normal_dir);
    vec3 L = normalize(light_dir);
    vec3 V = normalize(view_dir);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(N, L), 0.0) * k_d;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * k_s;

    // Write final color to the framebuffer
    color = vec4(ambient + diffuse + specular, 1.0);
}
