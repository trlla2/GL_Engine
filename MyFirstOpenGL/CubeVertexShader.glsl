#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 aTexCoord; // coordenadas UV del modelo
layout(location = 2) in vec3 aNormal; // normales del modelo


out vec2 vTexCoord; // se propaga al geometry shader
out vec3 vNormal; // se propaga al geometry shader

void main()
{
    vNormal   = aNormal;
    gl_Position = vec4(posicion, 1.0); // 
    vTexCoord = aTexCoord;
}