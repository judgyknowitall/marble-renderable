#version 410 core

// Input
layout (location = 0) in vec4 position;

// Transformation Matrices
uniform mat4 lightView;
uniform mat4 lightProjection;
uniform mat4 model;

void main()
{
    mat4 lightSpaceMatrix = lightProjection * lightView;
    gl_Position = lightSpaceMatrix * model * position;
} 