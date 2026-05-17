#version 440 core

// fragment shader con textura y tint multiplicador
in vec2 gTexCoord; // UVs recibidas del geometry shader
out vec4 fragColor;

uniform sampler2D uTexture; // textura de la instancia
uniform vec4 uTint; // (1,1,1,1 = sin cambio de color)

void main()
{
    vec4 texColor = texture(uTexture, gTexCoord);
    fragColor = texColor * uTint;
}