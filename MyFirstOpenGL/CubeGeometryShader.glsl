#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec2 vTexCoord[]; // recibe UVs del vertex shader por cada vértice

out vec2 gTexCoord; // propaga las UVs al fragment shader

out vec3 gNormal;
out vec4 gPrimitivePosition;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    //Matrices de transformación
    mat4 model = translationMatrix * rotationMatrix * scaleMatrix;

    //Calcular la posición del centro del triangulo
    gPrimitivePosition = model * ((gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) * 0.33);

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = projectionMatrix * viewMatrix * model * gl_in[i].gl_Position;
        gTexCoord   = vTexCoord[i];
        gNormal     = mat3(rotationMatrix) * vNormal[i];
        EmitVertex();
    }

    EndPrimitive();
}