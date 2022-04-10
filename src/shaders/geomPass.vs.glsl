#version 410 core

// Inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
//out vec4 Color;
out vec2 TexCoord;
//out float c;

// Transformation Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    //Color = color;
    TexCoord = texCoord;
    //TexCoord = vec2(1.f, 1.f);

    Normal = mat3(view * model) * normal;

    vec4 worldPos = model * position;
    gl_Position = projection * view * worldPos;
    gl_Position = gl_Position / gl_Position.w;
    FragPos = gl_Position.xyz;

    //c = model * 255 * sin(FragPos.x + FragPos.y);



}