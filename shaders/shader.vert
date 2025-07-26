#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;

struct Object {
    float radius;
    vec2 position;
    vec2 velocity;
};

restrict layout (binding = 2, std430) buffer AllObjectsLayout
{
    Object[] allObjects;
};

void main()
{
    // gl_Position = ortho * view * model * vec4(aPos, 1.0);
    // Object currentObject = allObjects[gl_InstanceID];
    // gl_Position = view * model * vec4(currentObject.position, 0.0, 1.0);
    gl_Position = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}