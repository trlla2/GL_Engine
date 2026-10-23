#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 vTexCoord;
out vec3 vNormal;

void main()
{
    vNormal   = aNormal;
    gl_Position = vec4(posicion, 1.0);
    vTexCoord = aTexCoord;
}
