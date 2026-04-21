#version 440 core

uniform vec2 windowSize;

out vec4 fragColor;

void main() 
{
        //Claude ha dado los valores de los colores (soy un vago)
        vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
        vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
        
        fragColor = (gl_FragCoord.y > windowSize.y / 2.0) ? yellow : orange;
}