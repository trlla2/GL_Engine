#version 440 core

uniform vec2 windowSize;
uniform int colorId;

out vec4 fragColor;

void main() 
{
        if(colorId == 0)
        {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        else if(colorId == 1)
        {
            fragColor = vec4(0.0, 1.0, 0.0, 1.0);
        }
        else if(colorId == 2)
        {
            fragColor = vec4(0.0, 0.0, 1.0, 1.0);
        }
        else
        {

            fragColor = vec4(1.0, 1.0, 1.0, 1.0);    
        }
}
