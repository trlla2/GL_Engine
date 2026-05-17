#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in  vec2 vTexCoord[]; // recibe UVs del vertex shader
out vec2 gTexCoord;   // las propaga al fragment shader

void main(){
    for(int i = 0; i < gl_in.length(); i++){
        gl_Position = gl_in[i].gl_Position;
        gTexCoord   = vTexCoord[i];
        EmitVertex();
    }
    EndPrimitive();
}