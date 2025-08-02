struct Object {
    vec2 position;
    vec2 velocity;
    float radius;
};

restrict layout (binding = 0, std430) buffer ObjectInformationBuffer
{
    Object[] allObjects;
};

// a hack so that the linter doesn't freak out <3
#line -1 1
#version 460 core