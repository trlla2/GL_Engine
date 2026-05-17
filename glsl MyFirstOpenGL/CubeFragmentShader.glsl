#version 440 core

// fragment shader con textura y tint multiplicador
in  vec2 gTexCoord;
out vec4 fragColor;

uniform sampler2D uTexture;
uniform vec4      uTint;     // (1,1,1,1) = sin cambio de color

void main()
{
    vec4 texColor = texture(uTexture, gTexCoord);
    fragColor     = texColor * uTint;
}