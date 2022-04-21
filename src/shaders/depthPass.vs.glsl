#version 410 core

// Input
layout (location = 0) in vec4 position;

// Transformation Matrices
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * position;
} 