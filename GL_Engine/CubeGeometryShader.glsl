#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec2 vTexCoord[];

out vec2 gTexCoord;
out vec3 gNormal;
out vec4 gPrimitivePosition;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    mat4 model = translationMatrix * rotationMatrix * scaleMatrix;

    // Centro del triangulo en espacio de mundo, usado por la linterna
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
