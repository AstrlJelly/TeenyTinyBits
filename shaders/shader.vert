#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;

struct Object {
    vec2 position;
    vec2 velocity;
    float radius;
};

restrict layout (binding = 0, std430) buffer AllObjectsLayout
{
    Object[] allObjects;
};

void main()
{
    Object currentObject = allObjects[gl_InstanceID];
    gl_Position = view * model * (vec4(aPos + vec3(currentObject.position, 1.0), 1.0));
    TexCoords = aTexCoords;
}