#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(){

	for(int i = 0; i < gl_in.length(); i++){
		gl_Position = projectionMatrix * viewMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}