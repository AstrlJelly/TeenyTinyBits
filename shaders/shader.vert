#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// projection * view * model
uniform mat4 relativeModel;

void main()
{
    gl_Position = relativeModel * vec4(aPos, 1.0);
} 