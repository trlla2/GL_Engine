#version 440 core

layout(location = 0) in vec3 posicion;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    mat4 model = translationMatrix * rotationMatrix * scaleMatrix;
    gl_Position = projectionMatrix * viewMatrix * model * vec4(posicion, 1.0);
}