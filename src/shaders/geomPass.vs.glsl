#version 410 core

// Inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec4 LightFragPos;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

// Transformation Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Light Space Matrix
uniform mat4 lightSpace;

void main()
{
    TexCoord = texCoord;
    Normal = mat3(view * model) * normal;

    vec4 worldPos = model * position;
    gl_Position = projection * view * worldPos;
    gl_Position = gl_Position / gl_Position.w;
    FragPos = gl_Position.xyz;

    LightFragPos = lightSpace * worldPos;
}