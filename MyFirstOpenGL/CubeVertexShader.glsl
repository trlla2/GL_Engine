#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 aTexCoord; // coordenadas UV del modelo

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 vTexCoord; // se propaga al geometry shader

void main()
{
    mat4 model = translationMatrix * rotationMatrix * scaleMatrix;
    gl_Position = projectionMatrix * viewMatrix * model * vec4(posicion, 1.0);
    vTexCoord = aTexCoord;
}