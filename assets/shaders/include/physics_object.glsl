struct Object {
    vec2 position;
    vec2 velocity;
    float radius;
};

restrict layout (binding = 0, std430) buffer ObjectInformationBuffer
{
    Object[] allObjects;
};