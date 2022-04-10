#version 410 core

// Inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;


// Outputs
out vec3 normal_dir;
out vec3 light_dir;
out vec3 view_dir;


// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_pos = vec3(1.0, 1.0, -1.0);    // Position of light



void main(void)
{
    mat4 mv_matrix = view * model;

    // Calculate view-space coordinate
    vec4 P = mv_matrix * position;

    // Calculate normal in view-space
    normal_dir = mat3(mv_matrix) * normal;

    // Calculate light vector
    light_dir = light_pos - P.xyz;

    // Calculate view vector
    view_dir = -P.xyz;

    // Calculate the clip-space position of each vertex
    gl_Position = projection * P;
	gl_Position = gl_Position / gl_Position.w;
}
