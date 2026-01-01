#version 460 core
#extension GL_ARB_shading_language_include : require

#include "/physics_object.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;

void main()
{
    Object currentObject = allObjects[gl_InstanceID];
    gl_Position = view * model * (vec4(aPos + vec3(currentObject.position, 1.0), 1.0));
    TexCoords = aTexCoords;
}