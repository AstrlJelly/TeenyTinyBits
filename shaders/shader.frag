#version 420 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform float time;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    float normalizedSin = (sin(time - (3.14/2)) + 1) / 2;
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), normalizedSin);
}